#include "brook.h"
#include "bk_httpsrv_cfg.h"
#include "bk_httpsrv.h"

struct bk_httpsrv *bk_httpsrv_new(void) {
    struct bk_httpsrv *srv = bk_alloc(sizeof(struct bk_httpsrv));
    if (!srv)
        return NULL;
    srv->cfg = bk__bk_httpsrv_cfg_new();
    if (!srv->cfg) {
        bk_httpsrv_free(srv);
        return NULL;
    }
    return srv;
}

void bk_httpsrv_free(struct bk_httpsrv *srv) {
    if (!srv)
        return;
    bk__bk_httpsrv_cfg_free(srv->cfg);
    bk_free(srv);
}

int bk_httpsrv_setopt_va(struct bk_httpsrv *srv, va_list ap) {
    (void) srv;
    (void) ap;
    return 0;
}

int bk_httpsrv_setopt(struct bk_httpsrv *srv, ...) {
    (void) srv;
    return 0;
}
