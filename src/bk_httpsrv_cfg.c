#include "brook.h"
#include "bk_httpsrv_cfg.h"

struct bk_httpsrv_cfg *bk__bk_httpsrv_cfg_new(void) {
    struct bk_httpsrv_cfg *cfg;
    cfg = bk_alloc(sizeof(struct bk_httpsrv_cfg));
    return cfg;
}

void bk__bk_httpsrv_cfg_free(struct bk_httpsrv_cfg *cfg) {
    bk_free(cfg);
}
