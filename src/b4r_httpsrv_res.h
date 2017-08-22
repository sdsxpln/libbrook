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