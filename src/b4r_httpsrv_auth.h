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