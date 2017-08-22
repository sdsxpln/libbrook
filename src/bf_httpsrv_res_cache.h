#ifndef BF_HTTPSRV_RES_CACHE_H
#define BF_HTTPSRV_RES_CACHE_H

#include <stdbool.h>
#include "b4r_hs.h"

struct bf_httpsrv_res_cache {
    struct b4r_hs *req_headers;
    struct b4r_hs *res_headers;
    bool active;
    bool weak;
};

struct bf_httpsrv_res_cache *_bf_httpsrv_res_cache_new(struct b4r_hs *req_headers, struct b4r_hs *res_headers);

void _bf_httpsrv_res_cache_free(struct bf_httpsrv_res_cache *cache);

#endif