#include "brook.h"
#include "bk_httpsrv.h"

struct bk_httpsrv *bk_httpsrv_new2(bk_httpsrv_req_cb req_cb, void *req_cls,
                                   bk_httpsrv_err_cb err_cb, void *err_cls) {
    struct bk_httpsrv *srv = bk_alloc(sizeof(struct bk_httpsrv));
    (void) req_cb;
    (void) req_cls;
    (void) err_cb;
    (void) err_cls;
    return srv;
}

struct bk_httpsrv *bk_httpsrv_new(bk_httpsrv_req_cb req_cb, void *req_cls) {
    (void) req_cb;
    (void) req_cls;
    return NULL;
}

void bk_httpsrv_free(struct bk_httpsrv *srv) {
    bk_free(srv);
}

int bk_httpsrv_start(struct bk_httpsrv *srv, unsigned int port, bool threaded) {
    (void) srv;
    (void) port;
    (void) threaded;
    return 0;
}

int bk_httpsrv_stop(struct bk_httpsrv *srv) {
    (void) srv;
    return 0;
}
