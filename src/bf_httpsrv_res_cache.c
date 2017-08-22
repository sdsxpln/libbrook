#include <microhttpd.h>
#include "b4r_macros.h"
#include "b4r_utils.h"
#include "bf_httpsrv_res_cache.h"

struct bf_httpsrv_res_cache *_bf_httpsrv_res_cache_new(struct b4r_hs *req_headers, struct b4r_hs *res_headers) {
    struct bf_httpsrv_res_cache *cache;
    _B4R_NEW(&cache);
    cache->req_headers = req_headers;
    cache->res_headers = res_headers;
    cache->active = true; //TODO: criar APIs bf_httpsrv_res_cache(bool active)
    return cache;
}

void _bf_httpsrv_res_cache_free(struct bf_httpsrv_res_cache *cache) {
    _B4R_FREE(cache);
}

bool bf_httpsrv_res_cache_is_fresh(struct bf_httpsrv_res_cache *cache) {
    const char *modified_since, *none_match, /**etag,*/ *cc;
    if (!cache || b4r_hs_count(cache->req_headers) == 0 || b4r_hs_count(cache->res_headers) == 0)
        return false;
    modified_since = b4r_hs_find_val(cache->req_headers, MHD_HTTP_HEADER_IF_MODIFIED_SINCE);
    none_match = b4r_hs_find_val(cache->req_headers, MHD_HTTP_HEADER_IF_NONE_MATCH);
    if (b4r_is_empty(modified_since) && b4r_is_empty(none_match))
        return false;
    cc = b4r_hs_find_val(cache->req_headers, MHD_HTTP_HEADER_CACHE_CONTROL);
    if (b4r_index(cc, "no-cache") != -1)
        return false;
    return true;
}

bool bf_httpsrv_res_cache_is_range_fresh() {
    return false;
}