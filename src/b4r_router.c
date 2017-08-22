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

#include <stddef.h>
#include "b4r_router.h"
#include <brook.h>

struct b4r_router *b4r_router_new() {
    return NULL;
}

void b4r_router_free(struct b4r_router *rtr) {

}

void b4r_router_route(struct b4r_router *rtr, void *req, void *res) {

}

void b4r_router_reg_sv_cb(b4r_router_cb cb, void *cls) {

}

void b4r_router_reg_req_cb(b4r_router_cb cb, void *cls) {

}