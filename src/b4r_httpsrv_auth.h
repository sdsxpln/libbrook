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

#ifndef B4R_HTTPSRV_AUTH_H
#define B4R_HTTPSRV_AUTH_H

#include <stdbool.h>
#include <microhttpd.h>
#include <brook.h>

typedef bool (*_b4r_httpsrv_auth_handle)(struct b4r_httpsrv_req *req, int *ret);

struct _b4r_httpsrv_auth {
    bool active;
    enum B4R_HTTPSRV_AUTH_TYPE type;
    char *realm, *err, *user, *pwd;
    _b4r_httpsrv_auth_handle handle;
};

struct _b4r_httpsrv_auth *_b4r_httpsrv_auth_new();

void _b4r_httpsrv_auth_free(struct _b4r_httpsrv_auth *auth);

bool _b4r_httpsrv_auth_basic_handle(struct b4r_httpsrv_req *req, int *ret);

bool _b4r_httpsrv_auth_digest_handle(struct b4r_httpsrv_req *req, int *ret);

#endif