#ifndef BF_HTTPSRV_CFG_H
#define BF_HTTPSRV_CFG_H

#include <brook.h>

struct bf_httpsrv_cfg {
    char *filename;
    size_t post_buffer_size;
    unsigned long max_body_size;
    unsigned long max_payld_size;
    uint64_t max_upld_size;
    char *content_type;
    char *uplds_dir;
    bf_uuid_func uuid_func;
    uint16_t port;
};

#endif