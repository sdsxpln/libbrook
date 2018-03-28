#include <stdio.h>
#include <errno.h>
#include "brook.h"
#include "bk_httpsrv.h"

static void bk__httpsrv_err_cb(void *cls, const char *err) {
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

static int bk__httpsrv_ahc(void *cls, struct MHD_Connection *con, const char *url, const char *method,
                           const char *version, const char *upld_data, size_t *upld_data_size, void **con_cls) {
    (void) cls;
    (void) con;
    (void) url;
    (void) method;
    (void) version;
    (void) upld_data;
    (void) upld_data_size;
    (void) con_cls;
    return MHD_NO;
}

struct bk_httpsrv *bk_httpsrv_new2(bk_httpsrv_req_cb req_cb, void *req_cls,
                                   bk_httpsrv_err_cb err_cb, void *err_cls) {
    struct bk_httpsrv *srv = bk_alloc(sizeof(struct bk_httpsrv));
    if (!req_cb || !err_cb)
        return NULL;
    srv->req_cb = req_cb;
    srv->req_cls = req_cls;
    srv->err_cb = err_cb;
    srv->err_cls = err_cls;
    return srv;
}

struct bk_httpsrv *bk_httpsrv_new(bk_httpsrv_req_cb req_cb, void *req_cls) {
    return bk_httpsrv_new2(req_cb, req_cls, bk__httpsrv_err_cb, NULL);
}

void bk_httpsrv_free(struct bk_httpsrv *srv) {
    if (!srv)
        return;
    if (srv->daemon)
        bk_httpsrv_stop(srv);
    bk_free(srv);
}

int bk_httpsrv_start(struct bk_httpsrv *srv, unsigned short port, bool threaded) {
    (void) threaded;
    if (!srv || port <= 0)
        return -EINVAL;
    if (!srv->daemon) {
        srv->daemon = MHD_start_daemon(
                MHD_USE_DUAL_STACK | MHD_USE_ERROR_LOG | (threaded ? MHD_USE_AUTO_INTERNAL_THREAD :
                                                          MHD_USE_THREAD_PER_CONNECTION |
                                                          MHD_USE_INTERNAL_POLLING_THREAD),
                port, NULL, NULL,
                bk__httpsrv_ahc, srv,
                MHD_OPTION_EXTERNAL_LOGGER, bk__httpsrv_oel, srv,
                MHD_OPTION_END);
        if (!srv->daemon)
            return -errno;
    }
    return 0;
}

int bk_httpsrv_stop(struct bk_httpsrv *srv) {
    if (!srv)
        return -EINVAL;
    if (srv->daemon) {
        MHD_stop_daemon(srv->daemon);
        srv->daemon = NULL;
    }
    return 0;
}
