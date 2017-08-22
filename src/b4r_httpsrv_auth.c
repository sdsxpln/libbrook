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

#include "b4r_macros.h"
#include "b4r_httpsrv_auth.h"
#include "b4r_httpsrv_req.h"
#include "b4r_httpsrv.h"

#include <microhttpd.h>

struct _b4r_httpsrv_auth *_b4r_httpsrv_auth_new() {
    struct _b4r_httpsrv_auth *auth;
    _B4R_NEW(&auth);
    return auth;
}

void _b4r_httpsrv_auth_free(struct _b4r_httpsrv_auth *auth) {
    if (auth) {
        _B4R_FREE(auth->realm);
        _B4R_FREE(auth->err);
        _B4R_FREE(auth->user);
        _B4R_FREE(auth->pwd);
        _B4R_FREE(auth);
    }
}

bool _b4r_httpsrv_auth_basic_handle(struct b4r_httpsrv_req *req, int *ret) {
    struct MHD_Response *res;
    char *user, *pwd;
    bool fail;
    pwd = NULL;
    user = MHD_basic_auth_get_username_password(req->con, &pwd);
    //TODO: "b4r_httpsrv_auth_cb" event?
    fail = !user || strcmp(user, req->owner->auth->user) != 0 || strcmp(pwd, req->owner->auth->pwd) != 0;
    free(user);
    free(pwd);
    if (fail) {
        res = MHD_create_response_from_buffer(strlen(req->owner->auth->err), req->owner->auth->err,
                                              MHD_RESPMEM_PERSISTENT);
        *ret = MHD_queue_basic_auth_fail_response(req->con, req->owner->auth->realm, res);
        MHD_destroy_response(res);
    }
    return fail;
}

bool _b4r_httpsrv_auth_digest_handle(struct b4r_httpsrv_req *req, int *ret) {
    return MHD_NO;
}