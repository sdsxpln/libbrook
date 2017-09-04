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

#ifndef B4R_HTTPSRV_RES_CACHE_H
#define B4R_HTTPSRV_RES_CACHE_H

#include <stdbool.h>
#include "b4r_hs.h"

struct b4r_httpsrv_res_cache {
    struct b4r_hs *req_headers;
    struct b4r_hs *res_headers;
    bool active;
    bool weak;
};

struct b4r_httpsrv_res_cache *_b4r_httpsrv_res_cache_new(struct b4r_hs *req_headers, struct b4r_hs *res_headers);

void _b4r_httpsrv_res_cache_free(struct b4r_httpsrv_res_cache *cache);

#endif