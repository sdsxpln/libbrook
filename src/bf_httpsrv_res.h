#ifndef BF_HTTPSRV_RES_H
#define BF_HTTPSRV_RES_H

#include <stdint.h>
#include "utstring.h"
#include "bf_httpsrv_res_cache.h"

struct bf_httpsrv_res {
    struct b4r_httpsrv_req *req;
    struct bf_httpsrv_res_cache *cache;
    struct b4r_hs *headers;
    const char *content_type;
    char *content_file;
    uint16_t status_code;
    UT_string *body;
};

struct bf_httpsrv_res *_bf_httpsrv_res_new(struct b4r_httpsrv_req *req);

void _bf_httpsrv_res_free(struct bf_httpsrv_res *res);

int _bf_httpsrv_res_dispatch(struct b4r_httpsrv_req *req);

#endif