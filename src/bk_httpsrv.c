#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "microhttpd.h"
#include "brook.h"
#if defined(__MINGW32__) || defined(__ANDROID__)
#include "bk_utils.h"
#endif
#include "bk_httpsrv.h"

static void bk__httperr_cb(void *cls, const char *err) {
    (void) cls;
    fprintf(stderr, "%s", err);
    fflush(stderr);
}

static void bk__httpsrv_oel(void *cls, const char *fmt, va_list ap) {
    struct bk_httpsrv *srv = cls;
    char err[256];
    vsprintf(err, fmt, ap);
    srv->err_cb(srv->err_cls, err);
}

static void bk__httpres_init(struct MHD_Connection *con, struct bk_httpres *res) {
    memset(res, 0, sizeof(struct bk_httpres));
    res->con = con;
    res->result = MHD_NO;
}

static void bk__httpres_done(struct bk_httpres res) {
    MHD_destroy_response(res.handle);
    bk_strmap_cleanup(&res.headers);
}

static int bk__httpsrv_ahc(void *cls, struct MHD_Connection *con, const char *url, const char *method,
                           const char *version, const char *upld_data, size_t *upld_data_size, void **con_cls) {
    struct bk_httpsrv *srv = cls;
    struct bk_httpres res;
    (void) url;
    (void) version;
    (void) method;
    (void) upld_data;
    (void) upld_data_size;
    if (!*con_cls) {
        *con_cls = (void *) 1;
        return MHD_YES;
    }
    *con_cls = NULL;
    bk__httpres_init(con, &res);
    srv->req_cb(srv->req_cls, NULL, &res);
    bk__httpres_done(res);
    return res.result;
}

static ssize_t bk__httpfileread_cb(void *cls, __BK_UNUSED uint64_t offset, char *buf, size_t size) {
    return fread(buf, sizeof(char), size, cls);
}

static void bk__httpfilefree_cb(void *cls) {
    fclose(cls);
}

static int bk__httpheaders_iter(void *cls, struct bk_strmap *header) {
    if (MHD_add_response_header(cls, header->name, header->val) == MHD_YES)
        return 0;
    return -ENOMEM;
}

struct bk_httpsrv *bk_httpsrv_new2(bk_httpreq_cb req_cb, void *req_cls, bk_httperr_cb err_cb, void *err_cls) {
    struct bk_httpsrv *srv = bk_alloc(sizeof(struct bk_httpsrv));
    if (!req_cb || !err_cb)
        return NULL;
    srv->req_cb = req_cb;
    srv->req_cls = req_cls;
    srv->err_cb = err_cb;
    srv->err_cls = err_cls;
    return srv;
}

struct bk_httpsrv *bk_httpsrv_new(bk_httpreq_cb cb, void *cls) {
    return bk_httpsrv_new2(cb, cls, bk__httperr_cb, NULL);
}

void bk_httpsrv_free(struct bk_httpsrv *srv) {
    if (!srv)
        return;
    bk_httpsrv_stop(srv);
    bk_free(srv);
}

int bk_httpsrv_start(struct bk_httpsrv *srv, uint16_t port, bool threaded) {
    if (!srv || port <= 0)
        return -EINVAL;
    if (!(srv->handle = MHD_start_daemon(
            MHD_USE_DUAL_STACK | MHD_USE_ERROR_LOG |
            (threaded ? MHD_USE_AUTO_INTERNAL_THREAD : MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD),
            port, NULL, NULL,
            bk__httpsrv_ahc, srv,
            MHD_OPTION_EXTERNAL_LOGGER, bk__httpsrv_oel, srv,
            MHD_OPTION_END)))
        return -ECONNABORTED;
    return 0;
}

int bk_httpsrv_stop(struct bk_httpsrv *srv) {
    if (!srv)
        return -EINVAL;
    if (srv->handle) {
        MHD_stop_daemon(srv->handle);
        srv->handle = NULL;
    }
    return 0;
}

struct bk_strmap **bk_httpres_headers(struct bk_httpres *res) {
    if (!res)
        return NULL;
    return &res->headers;
}

int bk_httpres_send(struct bk_httpres *res, const char *val, const char *content_type, unsigned int status) {
    if (!val)
        return -EINVAL;
    return bk_httpres_sendbinary(res, (void *) val, strlen(val), content_type, status);
}

int bk_httpres_sendbinary(struct bk_httpres *res, void *buf, size_t size, const char *content_type,
                          unsigned int status) {
    int ret;
    if (!res || !buf || !content_type || status <= 0)
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(res->handle = MHD_create_response_from_buffer(size, buf, MHD_RESPMEM_MUST_COPY)))
        return -ENOMEM;
    if ((ret = bk_strmap_set(&res->headers, MHD_HTTP_HEADER_CONTENT_TYPE, content_type)) != 0)
        return ret;
    if ((ret = bk_strmap_iter(res->headers, bk__httpheaders_iter, res->handle)) != 0)
        return ret;
    res->result = MHD_queue_response(res->con, status, res->handle);
    return 0;
}

int bk_httpres_sendstr(struct bk_httpres *res, struct bk_str *str, const char *content_type,
                       unsigned int status) {
    if (!str)
        return -EINVAL;
    return bk_httpres_sendbinary(res, (void *) bk_str_content(str), bk_str_length(str), content_type, status);
}

int bk_httpres_sendfile(struct bk_httpres *res, const char *filename, bool rendered) {
    FILE *file;
    struct stat buf;
    char attach_filename[256];
    int fd, ret;
    if (!res || !filename)
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(file = fopen(filename, "rb")))
        return -errno;
    if ((fd = fileno(file)) == -1) {
        ret = -errno;
        goto failed;
    }
    if (fstat(fd, &buf)) {
        ret = -errno;
        goto failed;
    }
    if (S_ISDIR(buf.st_mode)) {
        ret = -EISDIR;
        goto failed;
    }
    if (!S_ISREG(buf.st_mode)) {
        ret = -EBADF;
        goto failed;
    }
    snprintf(attach_filename, sizeof(attach_filename), "%s; filename=\"%s\"", (rendered ? "inline" : "attachment"),
             basename(filename));
    if ((ret = bk_strmap_set(&res->headers, MHD_HTTP_HEADER_CONTENT_DISPOSITION, attach_filename)) != 0)
        goto failed;
    if ((ret = bk_httpres_sendstream(res, (uint64_t) buf.st_size, 32768, bk__httpfileread_cb, file,
                                     bk__httpfilefree_cb)) != 0)
        goto failed;
    return 0;
failed:
    fclose(file);
    return ret;
}

int bk_httpres_sendstream(struct bk_httpres *res, uint64_t size, size_t block_size, bk_httpread_cb write_cb, void *cls,
                          bk_httpfree_cb flush_cb) {
    int ret;
    if (!res)
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(res->handle = MHD_create_response_from_callback(size, block_size, write_cb, cls, flush_cb)))
        return -ENOMEM;
    if (res->headers && (ret = bk_strmap_iter(res->headers, bk__httpheaders_iter, res->handle)) != 0)
        return ret;
    res->result = MHD_queue_response(res->con, MHD_HTTP_OK, res->handle);
    return 0;
}

int bk_httpres_senddata(struct bk_httpres *res, size_t block_size, bk_httpread_cb read_cb, void *cls,
                        bk_httpfree_cb free_cb) {
    return bk_httpres_sendstream(res, MHD_SIZE_UNKNOWN, block_size, read_cb, cls, free_cb);
}
