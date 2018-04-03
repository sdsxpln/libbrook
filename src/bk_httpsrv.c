#include <stdio.h>
#include <errno.h>
#include "microhttpd.h"
#include "brook.h"
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
    res->ret = MHD_NO;
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
    return res.ret;
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
    (void) threaded;
    if (!srv || port <= 0)
        return -EINVAL;
    if (!srv->handle) {
        srv->handle = MHD_start_daemon(
                MHD_USE_DUAL_STACK | MHD_USE_ERROR_LOG | (threaded ? MHD_USE_AUTO_INTERNAL_THREAD :
                                                          MHD_USE_THREAD_PER_CONNECTION |
                                                          MHD_USE_INTERNAL_POLLING_THREAD),
                port, NULL, NULL,
                bk__httpsrv_ahc, srv,
                MHD_OPTION_EXTERNAL_LOGGER, bk__httpsrv_oel, srv,
                MHD_OPTION_END);
        if (!srv->handle)
            return -errno;
    }
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

int bk_httpres_send(struct bk_httpres *res, const char *val, const char *content_type, unsigned int status) {
    if (!val)
        return -EINVAL;
    bk_httpres_sendbinary(res, (void *) val, strlen(val), content_type, status);
    return 0;
}

int bk_httpres_sendbinary(struct bk_httpres *res, void *buffer, size_t size, const char *content_type,
                          unsigned int status) {
    if (!res || !buffer || !content_type || status <= 0)
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    res->handle = MHD_create_response_from_buffer(size, buffer, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(res->handle, MHD_HTTP_HEADER_CONTENT_TYPE, content_type);
    res->ret = MHD_queue_response(res->con, status, res->handle);
    MHD_destroy_response(res->handle);
    return 0;
}

int bk_httpres_sendstr(struct bk_httpres *res, struct bk_str *str, const char *content_type,
                       unsigned int status) {
    if (!str)
        return -EINVAL;
    bk_httpres_sendbinary(res, (void *) bk_str_content(str), bk_str_length(str), content_type, status);
    return 0;
}

int bk_httpres_sendfile(struct bk_httpres *res, const char *filename, bool rendered) {
    (void) res;
    (void) filename;
    (void) rendered;
    return 0;
}

/*int bk_httpres_sendstream(struct bk_httpres *res) {
    return 0;
}*/