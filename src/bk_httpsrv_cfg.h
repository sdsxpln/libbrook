#ifndef BK_HTTPSRV_CFG_H
#define BK_HTTPSRV_CFG_H

#include <stdint.h>

struct bk_httpsrv_cfg {
    uint16_t port;
};

struct bk_httpsrv_cfg *bk__bk_httpsrv_cfg_new(void);

void bk__bk_httpsrv_cfg_free(struct bk_httpsrv_cfg *cfg);

#endif /* BK_HTTPSRV_CFG_H */
