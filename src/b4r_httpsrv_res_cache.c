/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2017 Silvio Clecio, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://brookframework.org/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 */

#include <microhttpd.h>
#include "b4r_macros.h"
#include "b4r_utils.h"
#include "b4r_httpsrv_res_cache.h"

struct b4r_httpsrv_res_cache *_b4r_httpsrv_res_cache_new(struct b4r_hs *req_headers, struct b4r_hs *res_headers) {
    struct b4r_httpsrv_res_cache *cache;
    _B4R_NEW(&cache);
    cache->req_headers = req_headers;
    cache->res_headers = res_headers;
    cache->active = true; //TODO: create b4r_httpsrv_res_cache(bool active) APIs.
    return cache;
}

void _b4r_httpsrv_res_cache_free(struct b4r_httpsrv_res_cache *cache) {
    _B4R_FREE(cache);
}

bool b4r_httpsrv_res_cache_is_fresh(struct b4r_httpsrv_res_cache *cache) {
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

bool b4r_httpsrv_res_cache_is_range_fresh() {
    return false;
}