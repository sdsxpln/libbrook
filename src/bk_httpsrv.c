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

static void bk__httpres_free(struct bk_httpres *res) {
    if (!res)
        return;
    bk_str_free(res->body);
    bk_free(res->type);
    bk_free(res);
}

static struct bk_httpres *bk__httpres_new(void) {
    struct bk_httpres *res = bk_alloc(sizeof(struct bk_httpres));
    if (!res)
        return NULL;
    res->body = bk_str_new();
    res->type = strdup("text/html; charset=utf-8");
    res->status = MHD_HTTP_OK;
    if (!res->body || !res->type) {
        bk__httpres_free(res);
        return NULL;
    }
    return res;
}

static int bk__httpsrv_ahc(void *cls, struct MHD_Connection *con, const char *url, const char *method,
                           const char *version, const char *upld_data, size_t *upld_data_size, void **con_cls) {
    struct bk_httpsrv *srv = cls;
    struct bk_httpres *res;
    int ret;
    (void) cls;
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
    res = bk__httpres_new();
    srv->req_cb(srv->req_cls, NULL, res);
    if (res->status <= 0)
        return MHD_NO;
    res->handle = MHD_create_response_from_buffer(bk_str_length(res->body), (void *) bk_str_content(res->body),
                                                  MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(res->handle, MHD_HTTP_HEADER_CONTENT_TYPE, res->type);
    ret = MHD_queue_response(con, res->status, res->handle);
    MHD_destroy_response(res->handle);
    bk__httpres_free(res);
    return ret;
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
    if (srv->handle)
        bk_httpsrv_stop(srv);
    bk_free(srv);
}

int bk_httpsrv_start(struct bk_httpsrv *srv, unsigned short port, bool threaded) {
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

int bk_httpres_type(struct bk_httpres *res, const char *type) {
    if (!res || !type)
        return -EINVAL;
    res->type = strdup(type);
    return 0;
}

int bk_httpres_status(struct bk_httpres *res, unsigned int status) {
    if (!res)
        return -EINVAL;
    res->status = status;
    return 0;
}

struct bk_str *bk_httpres_body(struct bk_httpres *res) {
    if (!res)
        return NULL;
    return res->body;
}

int bk_httpres_download(struct bk_httpres *res, const char *filename, bool rendered) {
    (void) res;
    (void) filename;
    (void) rendered;
    return 0;
}
