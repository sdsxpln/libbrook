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
 * This file is part of Brook4-REST.
 *
 * Brook4-REST is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brook4-REST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Brook4-REST.  If not, see <http://www.gnu.org/licenses/>.
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