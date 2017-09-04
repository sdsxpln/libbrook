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

#ifndef B4R_HTTPSRV_RES_H
#define B4R_HTTPSRV_RES_H

#include <stdint.h>
#include "utstring.h"
#include "b4r_httpsrv_res_cache.h"

struct b4r_httpsrv_res {
    struct b4r_httpsrv_req *req;
    struct b4r_httpsrv_res_cache *cache;
    struct b4r_hs *headers;
    const char *content_type;
    char *content_file;
    uint16_t status_code;
    UT_string *body;
};

struct b4r_httpsrv_res *_b4r_httpsrv_res_new(struct b4r_httpsrv_req *req);

void _b4r_httpsrv_res_free(struct b4r_httpsrv_res *res);

int _b4r_httpsrv_res_dispatch(struct b4r_httpsrv_req *req);

#endif