#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "microhttpd.h"
#include "bk_macros.h"
#include "bk_utils.h"
#include "bk_strmap.h"
#include "bk_httpsrv.h"

static int bk__httpheaders_iter(void *cls, struct bk_strmap *header) {
    if (MHD_add_response_header(cls, header->name, header->val) != MHD_YES)
        return -ENOMEM;
    return 0;
}

static ssize_t bk__httpfileread_cb(void *cls, __BK_UNUSED uint64_t offset, char *buf, size_t size) {
    return
#ifdef __ANDROID__
        (ssize_t)
#endif
            fread(buf, sizeof(char), size, cls);
}

static void bk__httpfilefree_cb(void *cls) {
    fclose(cls);
}

static struct bk_httpreq *bk__httpreq_new(void) {
    return bk_alloc(sizeof(struct bk_httpreq));
}

static void bk__httpreq_cleanup(struct bk_httpreq *req) {
    bk_strmap_cleanup(&req->headers);
    bk_strmap_cleanup(&req->cookies);
    bk_strmap_cleanup(&req->params);
}

static int bk__httpreq_iter(void *cls, __BK_UNUSED enum MHD_ValueKind kind, const char *key, const char *val) {
    struct bk__httpconvals_holder *holder = cls;
    return (holder->failed = (bk_strmap_add(holder->map, key, val) != 0)) ? MHD_NO : MHD_YES;
}

static void bk__httpreq_init(struct bk_httpreq *req, struct MHD_Connection *con, const char *version,
                             const char *method, const char *path) {
    struct bk__httpconvals_holder holder;
    memset(&holder, 0, sizeof(struct bk__httpconvals_holder));
    holder.map = &req->headers;
    MHD_get_connection_values(con, MHD_HEADER_KIND, bk__httpreq_iter, &holder);
    if (holder.failed)
        goto fail_oom;
    holder.map = &req->cookies;
    MHD_get_connection_values(con, MHD_COOKIE_KIND, bk__httpreq_iter, &holder);
    if (holder.failed)
        goto fail_oom;
    holder.map = &req->params;
    MHD_get_connection_values(con, MHD_GET_ARGUMENT_KIND, bk__httpreq_iter, &holder);
    if (holder.failed)
        goto fail_oom;
    req->version = version;
    req->method = method;
    req->path = path;
    return;
fail_oom:
    bk__httpreq_cleanup(req);
    oom();
}

static void bk__httpreq_done(__BK_UNUSED void *cls, __BK_UNUSED struct MHD_Connection *con, void **con_cls,
                             __BK_UNUSED enum MHD_RequestTerminationCode toe) {
    struct bk_httpreq *req = *con_cls;
    if (req) {
        bk__httpreq_cleanup(req);
        bk_free(req);
    }
    *con_cls = NULL;
}

static void bk__httpres_init(struct bk_httpres *res, struct MHD_Connection *con) {
    memset(res, 0, sizeof(struct bk_httpres));
    res->con = con;
    res->status = 500;
}

static int bk__httpres_done(struct bk_httpres *res) {
    if (res->headers && bk_strmap_iter(res->headers, bk__httpheaders_iter, res->handle) != 0) {
        bk_strmap_cleanup(&res->headers);
        MHD_destroy_response(res->handle);
        oom();
    }
    bk_strmap_cleanup(&res->headers);
    res->ret = MHD_queue_response(res->con, res->status, res->handle);
    MHD_destroy_response(res->handle);
    return res->ret;
}

static void bk__httpauth_init(struct bk_httpauth *auth, struct MHD_Connection *con) {
    memset(auth, 0, sizeof(struct bk_httpauth));
    auth->con = con;
    auth->usr = MHD_basic_auth_get_username_password(con, &auth->pwd);
}

static bool bk__httpauth_done(struct bk_httpauth *auth) {
    bk_free(auth->usr);
    bk_free(auth->pwd);
    if (auth->ret) {
        auth->ret = MHD_YES;
        goto done;
    }
    if (auth->canceled) {
        auth->ret = MHD_NO;
        goto done;
    }
    auth->handle = MHD_create_response_from_buffer(strlen(auth->justification), auth->justification,
                                                   MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(auth->handle, MHD_HTTP_HEADER_CONTENT_TYPE, auth->content_type);
    bk_free(auth->content_type);
    auth->ret = MHD_queue_basic_auth_fail_response(auth->con, auth->realm ? auth->realm : _("Brook realm"),
                                                   auth->handle);
    bk_free(auth->realm);
    MHD_destroy_response(auth->handle);
    return false;
done:
    bk_free(auth->realm);
    bk_free(auth->justification);
    bk_free(auth->content_type);
    return auth->ret == MHD_YES;
}

static void bk__httperr_cb(__BK_UNUSED void *cls, const char *err) {
    if (isatty(fileno(stderr)) && (fprintf(stderr, "%s", err) > 0))
        fflush(stderr);
}

static void bk__httpsrv_oel(void *cls, const char *fmt, va_list ap) {
    struct bk_httpsrv *srv = cls;
    va_list ap_cpy;
    size_t size;
    char *err;
    va_copy(ap_cpy, ap);
    size = vsnprintf(NULL, 0, fmt, ap_cpy) + sizeof(char);
    va_end(ap_cpy);
    if (!(err = bk_alloc(size)))
        oom();
    vsnprintf(err, size, fmt, ap);
    srv->err_cb(srv->err_cls, err);
    bk_free(err);
}

static int bk__httpsrv_ahc(void *cls, struct MHD_Connection *con, const char *url, const char *method,
                           const char *version, const char *upld_data, size_t *upld_data_size, void **con_cls) {
    struct bk_httpsrv *srv = cls;
    struct bk_httpreq *req;
    struct bk_httpres res;
    struct bk_httpauth auth;

    (void) upld_data;
    (void) upld_data_size;

    if (!*con_cls) {
        if (srv->auth_cb) {
            bk__httpauth_init(&auth, con);
            auth.ret = srv->auth_cb(srv->auth_cls, &auth);
            if (!bk__httpauth_done(&auth))
                return auth.ret;
        }
        if (!(*con_cls = bk__httpreq_new()))
            oom();
        return MHD_YES;
    }
    req = *con_cls;
    bk__httpreq_init(req, con, version, method, url);
    bk__httpres_init(&res, con);
    srv->req_cb(srv->req_cls, req, &res);
    return bk__httpres_done(&res);
}

ssize_t bk_httpread_end(bool err) {
    return
#ifdef __ANDROID__
        (ssize_t)
#endif
            (err ? MHD_CONTENT_READER_END_WITH_ERROR : MHD_CONTENT_READER_END_OF_STREAM);
}

int bk_httpauth_setrealm(struct bk_httpauth *auth, const char *realm) {
    if (!auth || !realm)
        return -EINVAL;
    if (auth->realm)
        return -EALREADY;
    if (!(auth->realm = strdup(realm)))
        oom();
    return 0;
}

int bk_httpauth_deny(struct bk_httpauth *auth, const char *justification, const char *content_type) {
    if (!auth || !justification || !content_type)
        return -EINVAL;
    if (auth->justification)
        return -EALREADY;
    if (!(auth->justification = strdup(justification)) || !(auth->content_type = strdup(content_type))) {
        bk_free(auth->justification);
        bk_free(auth->content_type);
        oom();
    }
    return 0;
}

int bk_httpauth_cancel(struct bk_httpauth *auth) {
    if (!auth)
        return -EINVAL;
    auth->canceled = true;
    return 0;
}

const char *bk_httpauth_usr(struct bk_httpauth *auth) {
    return auth ? auth->usr : NULL;
}

const char *bk_httpauth_pwd(struct bk_httpauth *auth) {
    return auth ? auth->pwd : NULL;
}

struct bk_httpsrv *bk_httpsrv_new2(bk_httpauth_cb auth_cb, void *auth_cls, bk_httpreq_cb req_cb, void *req_cls,
                                   bk_httperr_cb err_cb, void *err_cls) {
    struct bk_httpsrv *srv = bk_alloc(sizeof(struct bk_httpsrv));
    if (!req_cb || !err_cb)
        return NULL;
    srv->auth_cb = auth_cb;
    srv->auth_cls = auth_cls;
    srv->req_cb = req_cb;
    srv->req_cls = req_cls;
    srv->err_cb = err_cb;
    srv->err_cls = err_cls;
    return srv;
}

struct bk_httpsrv *bk_httpsrv_new(bk_httpreq_cb cb, void *cls) {
    return bk_httpsrv_new2(NULL, NULL, cb, cls, bk__httperr_cb, NULL);
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
            (threaded ? MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_THREAD_PER_CONNECTION : MHD_USE_AUTO_INTERNAL_THREAD),
            port, NULL, NULL,
            bk__httpsrv_ahc, srv,
            MHD_OPTION_EXTERNAL_LOGGER, bk__httpsrv_oel, srv,
            MHD_OPTION_NOTIFY_COMPLETED, bk__httpreq_done, NULL,
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

struct bk_strmap **bk_httpreq_headers(struct bk_httpreq *req) {
    return req ? &req->headers : NULL;
}

struct bk_strmap **bk_httpreq_cookies(struct bk_httpreq *req) {
    return req ? &req->cookies : NULL;
}

struct bk_strmap **bk_httpreq_params(struct bk_httpreq *req) {
    return req ? &req->params : NULL;
}

const char *bk_httpreq_version(struct bk_httpreq *req) {
    return req ? req->version : NULL;
}

const char *bk_httpreq_method(struct bk_httpreq *req) {
    return req ? req->method : NULL;
}

const char *bk_httpreq_path(struct bk_httpreq *req) {
    return req ? req->path : NULL;
}

int bk_httpreq_setuserdata(struct bk_httpreq *req, void *data) {
    if (!req || !data)
        return -EINVAL;
    req->userdata = data;
    return 0;
}

void *bk_httpreq_userdata(struct bk_httpreq *req) {
    return req ? req->userdata : NULL;
}

struct bk_strmap **bk_httpres_headers(struct bk_httpres *res) {
    return res ? &res->headers : NULL;
}

int bk_httpres_send(struct bk_httpres *res, const char *val, const char *content_type, unsigned int status) {
    return bk_httpres_sendbinary(res, (void *) val, (val ? strlen(val) : 0), content_type, status);
}

int bk_httpres_sendbinary(struct bk_httpres *res, void *buf, size_t size, const char *content_type,
                          unsigned int status) {
    if (!res || !buf || !content_type || status <= 0)
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(res->handle = MHD_create_response_from_buffer(size, buf, MHD_RESPMEM_MUST_COPY)))
        oom();
    bk_strmap_set(&res->headers, MHD_HTTP_HEADER_CONTENT_TYPE, content_type);
    res->status = status;
    return 0;
}

int bk_httpres_sendstr(struct bk_httpres *res, struct bk_str *str, const char *content_type, unsigned int status) {
    return bk_httpres_sendbinary(res, (void *) bk_str_content(str), bk_str_length(str), content_type, status);
}

int bk_httpres_sendfile(struct bk_httpres *res, size_t block_site, uint64_t max_size, const char *filename,
                        bool rendered, unsigned int status) {
    FILE *file;
    struct stat64 sbuf;
    char *absolute_path;
    const char *cd_type;
    const char *cd_basename;
    char *cd_header;
    size_t fn_size;
    int fd, eno = 0;
    if (!res || !filename || block_site < 1)
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(absolute_path = realpath(filename, NULL)))
        return -errno;
#ifdef _WIN32
    eno = fopen_s(&file, absolute_path, "rb");
    if (eno)
        goto fail;
#else
    if (!(file = fopen(absolute_path, "rb"))) {
        eno = errno;
        goto fail;
    }
#endif
    if ((fd = fileno(file)) == -1) {
        eno = errno;
        goto fail;
    }
    if (fstat64(fd, &sbuf)) {
        eno = errno;
        goto fail;
    }
    if ((max_size > 0) && ((uint64_t) sbuf.st_size > max_size)) {
        eno = EFBIG;
        goto fail;
    }
    if (S_ISDIR(sbuf.st_mode)) {
        eno = EISDIR;
        goto fail;
    }
    if (!S_ISREG(sbuf.st_mode)) {
        eno = EBADF;
        goto fail;
    }
#define _BK_FNFMT "%s; filename=\"%s\""
    cd_type = rendered ? "inline" : "attachment";
    cd_basename = basename(absolute_path);
    fn_size = snprintf(NULL, 0, _BK_FNFMT, cd_type, cd_basename) + sizeof(char);
    if (!(cd_header = bk_alloc(fn_size))) {
        eno = ENOMEM;
        goto fail;
    }
    snprintf(cd_header, fn_size, _BK_FNFMT, cd_type, cd_basename);
    bk_free(absolute_path);
#undef _BK_FNFMT
    bk_strmap_set(&res->headers, MHD_HTTP_HEADER_CONTENT_DISPOSITION, cd_header);
    bk_free(cd_header);
    if (!(res->handle = MHD_create_response_from_callback((uint64_t) sbuf.st_size, block_site, bk__httpfileread_cb,
                                                          file, bk__httpfilefree_cb))) {
        eno = ENOMEM;
        goto fail;
    }
    res->status = status;
    return 0;
fail:
    bk_free(absolute_path);
    if (file) {
        if (eno == 0)
            eno = fclose(file);
        else
            fclose(file);
    }
    if (eno == ENOMEM)
        oom();
    return -eno;
}

int bk_httpres_sendstream(struct bk_httpres *res, uint64_t size, size_t block_size, bk_httpread_cb write_cb, void *cls,
                          bk_httpfree_cb free_cb, unsigned int status) {
    int eno;
    if (!res) {
        eno = EINVAL;
        goto failed;
    }
    if (res->handle) {
        eno = EALREADY;
        goto failed;
    }
    if (!(res->handle = MHD_create_response_from_callback(size, block_size, write_cb, cls, free_cb))) {
        eno = ENOMEM;
        goto failed;
    }
    res->status = status;
    return 0;
failed:
    if (free_cb)
        free_cb(cls);
    if (eno == ENOMEM)
        oom();
    return -eno;
}

int bk_httpres_senddata(struct bk_httpres *res, size_t block_size, bk_httpread_cb read_cb, void *cls,
                        bk_httpfree_cb free_cb, unsigned int status) {
    return bk_httpres_sendstream(res, MHD_SIZE_UNKNOWN, block_size, read_cb, cls, free_cb, status);
}
