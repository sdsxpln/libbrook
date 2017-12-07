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

#include <stdlib.h>
#include <string.h>
#include "utlist.h"
#include "b4r_httpsrv_strs.h"
#include "b4r_httpsrv_req_uplds.h"
#include "b4r_httpsrv_req.h"
#include "b4r_httpsrv_res.h"
#include "b4r_httpsrv.h"

#define _B4R_HTTPSRV_REQ_GET_PROP(el, mb, def) (el) ? (el)->mb : def

#define _B4R_HTTPSRV_REQ_REF(el, mb) (el) ? (&(el)->mb) : NULL

struct _b4r_hs_kv_holder {
    struct b4r_hs **hsl;
    bool ok;
};

void *_b4r_httpsrv_req_init_cb(void *cls, const char *uri, struct MHD_Connection *con) {
    struct b4r_httpsrv_req *req;
    _B4R_NEW(&req);
    req->owner = cls;
    req->res = _b4r_httpsrv_res_new(req);
    utstring_new(req->payload);
    req->con = con;
#ifndef DISABLE_CON_CB
    req->id = MHD_get_connection_info(con, MHD_CONNECTION_INFO_SOCKET_CONTEXT)->socket_context;
#endif
    req->uri = strdup(uri);
    if (req->owner->req_init_cb)
        req->owner->req_init_cb(req->owner->req_init_cls, req->id, req->uri, &req->aborted);
    return req;
}

void _b4r_httpsrv_req_fini_cb(void *cls, struct MHD_Connection *con, void **con_cls,
                              enum MHD_RequestTerminationCode toe) {
    struct b4r_httpsrv_req *req;
    if (con_cls) {
        req = *con_cls;
        _b4r_httpsrv_res_free(req->res);
        b4r_hs_cleanup(&req->headers);
        b4r_hs_cleanup(&req->cookies);
        b4r_hs_cleanup(&req->params);
        b4r_hs_cleanup(&req->fields);
        utstring_free(req->payload);
        MHD_destroy_post_processor(req->post_proc);
        _B4R_FREE(req->uri);
        _b4r_httpsrv_req_upds_cleanup(req->uplds);
        if (req->owner->req_fini_cb)
            req->owner->req_fini_cb(req->owner->req_fini_cls, toe == MHD_REQUEST_TERMINATED_COMPLETED_OK);
        _B4R_FREE(req);
        *con_cls = NULL;
    }
}

void _b4r_httpsrv_req_err(struct b4r_httpsrv_req *req, const char *err) {
    if (req->owner->req_err_cb) {
        utstring_clear(req->res->body);
        req->res->status_code = MHD_HTTP_INTERNAL_SERVER_ERROR;
        req->owner->req_err_cb(req->owner->req_err_cls, req, req->res, &req->done, err);
    }
}

void _b4r_httpsrv_req_errf(struct b4r_httpsrv_req *req, const char *fmt, ...) {
    va_list va;
    char err[B4R_ERR_SIZE];
    va_start(va, fmt);
    _b4r_httpsrv_req_err(req, err);
    va_end(va);
}

static int _b4r_httpsrv_req_iter_kv(void *cls, enum MHD_ValueKind kind, const char *key, const char *val) {
    struct _b4r_hs_kv_holder *hldr = cls;
    hldr->ok = b4r_hs_set(hldr->hsl, key, val);
    return hldr->ok;
}

static int _b4r_httpsrv_req_prepare(struct b4r_httpsrv_req *req, const char *url, const char *method,
                                    const char *version) {
    struct _b4r_hs_kv_holder *kv_hldr;
    req->is_post = b4r_is_post(method);
    if (req->is_post && !req->owner->req_allowed_post)
        return MHD_NO;
    req->receiving = true;
    req->done = true;
    req->version = version;
    req->method = method;
    req->path = url;
    req->content_type = MHD_lookup_connection_value(req->con, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_TYPE);
    req->content_len = b4r_str_to_size(
            MHD_lookup_connection_value(req->con, MHD_HEADER_KIND, MHD_HTTP_HEADER_CONTENT_LENGTH));
    _B4R_NEW(&kv_hldr);
    kv_hldr->hsl = &req->headers;
    MHD_get_connection_values(req->con, MHD_HEADER_KIND, &_b4r_httpsrv_req_iter_kv, kv_hldr);
    if (!kv_hldr->ok) {
        _b4r_httpsrv_req_err(req, S_B4R_HEADERS_FAILED);
        return MHD_NO;
    }
    kv_hldr->hsl = &req->cookies;
    MHD_get_connection_values(req->con, MHD_COOKIE_KIND, &_b4r_httpsrv_req_iter_kv, kv_hldr);
    if (!kv_hldr->ok) {
        _b4r_httpsrv_req_err(req, S_B4R_COOKIES_FAILED);
        return MHD_NO;
    }
    kv_hldr->hsl = &req->params;
    MHD_get_connection_values(req->con, MHD_GET_ARGUMENT_KIND, &_b4r_httpsrv_req_iter_kv, kv_hldr);
    if (!kv_hldr->ok) {
        _b4r_httpsrv_req_err(req, S_B4R_PARAMS_FAILED);
        return MHD_NO;
    }
    _B4R_FREE(kv_hldr);
    if (req->owner->req_prep_cb)
        req->owner->req_prep_cb(req->owner->req_prep_cls, req, req->res, &req->done);
    return MHD_YES;
}

int _b4r_httpsrv_req_cb(void *cls, struct MHD_Connection *con, const char *url, const char *method, const char *version,
                        const char *upld_data, size_t *upld_data_size, void **con_cls) {
    struct b4r_httpsrv_req *req = *con_cls;
    int ret;
    if (req->aborted)
        return MHD_NO;
    if (req->receiving) {
        if (_b4r_httpsrv_req_uplds_process(req, con, con_cls, upld_data, upld_data_size, &ret))
            return ret;
        return _b4r_httpsrv_res_dispatch(req);
    }
    if (req->owner->auth->active && req->owner->auth->handle(req, &ret))
        return ret;
    return _b4r_httpsrv_req_prepare(req, url, method, version);
}

void *b4r_httpsrv_req_owner(struct b4r_httpsrv_req *req) {
    return req ? req->owner : NULL;
}

const char *b4r_httpsrv_req_version(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, version, NULL);
}

const char *b4r_httpsrv_req_id(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, id, NULL);
}

const char *b4r_httpsrv_req_uri(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, uri, NULL);
}

const char *b4r_httpsrv_req_path(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, path, NULL);
}

const char *b4r_httpsrv_req_method(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, method, NULL);
}

const char *b4r_httpsrv_req_content_type(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, content_type, NULL);
}

size_t b4r_httpsrv_req_content_len(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_GET_PROP(req, content_len, 0);
}

bool b4r_httpsrv_req_is_post(struct b4r_httpsrv_req *req) {
    return (bool) (_B4R_HTTPSRV_REQ_GET_PROP(req, is_post, false));
}

struct b4r_hs **b4r_httpsrv_req_headers(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_REF(req, headers);
}

struct b4r_hs **b4r_httpsrv_req_params(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_REF(req, params);
}

struct b4r_hs **b4r_httpsrv_req_cookies(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_REF(req, cookies);
}

struct b4r_hs **b4r_httpsrv_req_fields(struct b4r_httpsrv_req *req) {
    return _B4R_HTTPSRV_REQ_REF(req, fields);
}

const char *b4r_httpsrv_req_payld(struct b4r_httpsrv_req *req) {
    return req ? utstring_body(req->payload) : NULL;
}

bool b4r_httpsrv_req_write(struct b4r_httpsrv_req *req, const char *buf, size_t size) {
    if (!req || !buf)
        return false;
    utstring_bincpy(req->payload, buf, size);
    return true;
}

bool b4r_httpsrv_req_send(struct b4r_httpsrv_req *req, const char *buf) {
    if (!req || !buf)
        return false;
    utstring_bincpy(req->payload, buf, strlen(buf));
    return true;
}

bool b4r_httpsrv_req_iter_uplds(struct b4r_httpsrv_req *req, b4r_httpsrv_req_uplds_iter_cb iter_cb, void *iter_cls) {
    struct b4r_httpsrv_req_upld *upld;
    if (!req || !iter_cb)
        return false;
    if (req->uplds)
        LL_FOREACH(req->uplds, upld) {
            iter_cb(iter_cls, upld);
        }
    return true;
}

#undef _B4R_HTTPSRV_REQ_GET_PROP

#undef _B4R_HTTPSRV_REQ_REF