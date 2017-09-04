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
#include "bf_httpsrv_strs.h"
#include "bf_httpsrv_req_uplds.h"
#include "bf_httpsrv_req.h"
#include "bf_httpsrv_res.h"
#include "bf_httpsrv.h"

#define _BF_HTTPSRV_REQ_GET_PROP(el, mb, def) (el) ? el->mb : def

#define _BF_HTTPSRV_REQ_GET_VAL(el, mb, name) (el) ? b4r_hs_find_val(el->mb, name) : NULL

#define _BF_HTTPSRV_REQ_TRY_VAL(el, mb, name, val) (el) && b4r_hs_try(el->mb, name, val)

#define _BF_HTTPSRV_REQ_ITER(el, mb, iter_cb, iter_cls) (el) && b4r_hs_iter(el->mb, iter_cb, iter_cls)

#define _BF_HTTPSRV_REQ_REF(el, mb) (el) ? el->mb : NULL

#define _BF_HTTPSRV_REQ_PAYLOAD(el) (el) ? utstring_body(el->payload) : NULL

struct _b4r_hs_kv_holder {
    struct b4r_hs **hsl;
    bool ok;
};

void *_bf_httpsrv_req_init_cb(void *cls, const char *uri, struct MHD_Connection *con) {
    struct b4r_httpsrv_req *req;
    _B4R_NEW(&req);
    req->owner = cls;
    req->res = _bf_httpsrv_res_new(req);
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

void _bf_httpsrv_req_fini_cb(void *cls, struct MHD_Connection *con, void **con_cls,
                             enum MHD_RequestTerminationCode toe) {
    struct b4r_httpsrv_req *req;
    if (con_cls) {
        req = *con_cls;
        _bf_httpsrv_res_free(req->res);
        b4r_hs_cleanup(&req->headers);
        b4r_hs_cleanup(&req->cookies);
        b4r_hs_cleanup(&req->params);
        b4r_hs_cleanup(&req->fields);
        utstring_free(req->payload);
        MHD_destroy_post_processor(req->post_proc);
        _B4R_FREE(req->uri);
        _bf_httpsrv_req_upds_cleanup(req->uplds);
        if (req->owner->req_fini_cb)
            req->owner->req_fini_cb(req->owner->req_fini_cls, toe == MHD_REQUEST_TERMINATED_COMPLETED_OK);
        _B4R_FREE(req);
        *con_cls = NULL;
    }
}

void _bf_httpsrv_req_err(struct b4r_httpsrv_req *req, const char *fmt, ...) {
    va_list va;
    if (req->owner->req_err_cb) {
        va_start(va, fmt);
        utstring_clear(req->res->body);
        req->res->status_code = MHD_HTTP_INTERNAL_SERVER_ERROR;
        req->owner->req_err_cb(req->owner->req_err_cls, req, req->res, &req->done, fmt, va);
        va_end(va);
    }
}

static int _bf_httpsrv_req_iter_kv(void *cls, enum MHD_ValueKind kind, const char *key, const char *val) {
    struct _b4r_hs_kv_holder *hldr = cls;
    hldr->ok = b4r_hs_add_or_set(hldr->hsl, key, val);
    return hldr->ok;
}

static int _bf_httpsrv_req_prepare(struct b4r_httpsrv_req *req, const char *url, const char *method,
                                   const char *version) {
    struct _b4r_hs_kv_holder *kv_hldr;
    req->uploading = b4r_is_post(method);
    if (req->uploading && !req->owner->req_allowed_post)
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
    MHD_get_connection_values(req->con, MHD_HEADER_KIND, &_bf_httpsrv_req_iter_kv, kv_hldr);
    if (!kv_hldr->ok) {
        _bf_httpsrv_req_err(req, S_BF_HEADERS_FAILED);
        return MHD_NO;
    }
    kv_hldr->hsl = &req->cookies;
    MHD_get_connection_values(req->con, MHD_COOKIE_KIND, &_bf_httpsrv_req_iter_kv, kv_hldr);
    if (!kv_hldr->ok) {
        _bf_httpsrv_req_err(req, S_BF_COOKIES_FAILED);
        return MHD_NO;
    }
    kv_hldr->hsl = &req->params;
    MHD_get_connection_values(req->con, MHD_GET_ARGUMENT_KIND, &_bf_httpsrv_req_iter_kv, kv_hldr);
    if (!kv_hldr->ok) {
        _bf_httpsrv_req_err(req, S_BF_PARAMS_FAILED);
        return MHD_NO;
    }
    _B4R_FREE(kv_hldr);
    if (req->owner->req_prep_cb)
        req->owner->req_prep_cb(req->owner->req_prep_cls, req, req->res, &req->done);
    return MHD_YES;
}

int _bf_httpsrv_req_cb(void *cls, struct MHD_Connection *con, const char *url, const char *method, const char *version,
                       const char *upld_data, size_t *upld_data_size, void **con_cls) {
    struct b4r_httpsrv_req *req = *con_cls;
    int ret;
    if (req->aborted)
        return MHD_NO;
    if (req->receiving) {
        if (_bf_httpsrv_req_uplds_process(req, con, con_cls, upld_data, upld_data_size, &ret))
            return ret;
        return _bf_httpsrv_res_dispatch(req);
    }
    if (req->owner->auth->active && req->owner->auth->handle(req, &ret))
        return ret;
    return _bf_httpsrv_req_prepare(req, url, method, version);
}

void *bf_httpsrv_req_owner(struct b4r_httpsrv_req *req) {
    return req ? req->owner : NULL;
}

const char *bf_httpsrv_req_version(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, version, NULL);
}

const char *bf_httpsrv_req_id(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, id, NULL);
}

const char *bf_httpsrv_req_uri(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, uri, NULL);
}

const char *bf_httpsrv_req_path(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, path, NULL);
}

const char *bf_httpsrv_req_method(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, method, NULL);
}

const char *bf_httpsrv_req_content_type(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, content_type, NULL);
}

size_t bf_httpsrv_req_content_len(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_GET_PROP(req, content_len, 0);
}

bool bf_httpsrv_req_up(struct b4r_httpsrv_req *req) {
    return (bool) (_BF_HTTPSRV_REQ_GET_PROP(req, uploading, false));
}

const char *bf_httpsrv_req_header(struct b4r_httpsrv_req *req, const char *name) {
    return _BF_HTTPSRV_REQ_GET_VAL(req, headers, name);
}

bool bf_httpsrv_req_try_header(struct b4r_httpsrv_req *req, const char *name, const char **val) {
    return _BF_HTTPSRV_REQ_TRY_VAL(req, headers, name, val);
}

bool bf_httpsrv_req_iter_headers(struct b4r_httpsrv_req *req, b4r_hs_iter_cb iter_cb, void *iter_cls) {
    return _BF_HTTPSRV_REQ_ITER(req, headers, iter_cb, iter_cls);
}

void *bf_httpsrv_req_headers_ref(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_REF(req, headers);
}

const char *bf_httpsrv_req_param(struct b4r_httpsrv_req *req, const char *name) {
    return _BF_HTTPSRV_REQ_GET_VAL(req, params, name);
}

bool bf_httpsrv_req_try_param(struct b4r_httpsrv_req *req, const char *name, const char **val) {
    return _BF_HTTPSRV_REQ_TRY_VAL(req, params, name, val);
}

bool bf_httpsrv_req_iter_params(struct b4r_httpsrv_req *req, b4r_hs_iter_cb iter_cb, void *iter_cls) {
    return _BF_HTTPSRV_REQ_ITER(req, params, iter_cb, iter_cls);
}

void *bf_httpsrv_req_params_ref(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_REF(req, params);
}

const char *bf_httpsrv_req_cookie(struct b4r_httpsrv_req *req, const char *name) {
    return _BF_HTTPSRV_REQ_GET_VAL(req, cookies, name);
}

bool bf_httpsrv_req_try_cookie(struct b4r_httpsrv_req *req, const char *name, const char **val) {
    return _BF_HTTPSRV_REQ_TRY_VAL(req, cookies, name, val);
}

bool bf_httpsrv_req_iter_cookies(struct b4r_httpsrv_req *req, b4r_hs_iter_cb iter_cb, void *iter_cls) {
    return _BF_HTTPSRV_REQ_ITER(req, cookies, iter_cb, iter_cls);
}

void *bf_httpsrv_req_cookies_ref(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_REF(req, cookies);
}

const char *bf_httpsrv_req_field(struct b4r_httpsrv_req *req, const char *name) {
    return _BF_HTTPSRV_REQ_GET_VAL(req, fields, name);
}

bool bf_httpsrv_req_try_field(struct b4r_httpsrv_req *req, const char *name, const char **val) {
    return _BF_HTTPSRV_REQ_TRY_VAL(req, fields, name, val);
}

bool bf_httpsrv_req_iter_fields(struct b4r_httpsrv_req *req, b4r_hs_iter_cb iter_cb, void *iter_cls) {
    return _BF_HTTPSRV_REQ_ITER(req, fields, iter_cb, iter_cls);
}

void *bf_httpsrv_req_fields_ref(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_REF(req, fields);
}

const char *bf_httpsrv_req_payld(struct b4r_httpsrv_req *req) {
    return _BF_HTTPSRV_REQ_PAYLOAD(req);
}

bool bf_httpsrv_req_write(struct b4r_httpsrv_req *req, const char *buf, size_t size) {
    if (!req || !buf)
        return false;
    utstring_bincpy(req->payload, buf, size);
    return true;
}

bool bf_httpsrv_req_send(struct b4r_httpsrv_req *req, const char *buf) {
    if (!req || !buf)
        return false;
    utstring_bincpy(req->payload, buf, strlen(buf));
    return true;
}

bool bf_httpsrv_req_iter_uplds(struct b4r_httpsrv_req *req, bf_httpsrv_req_uplds_iter_cb iter_cb, void *iter_cls) {
    struct bf_httpsrv_req_upld *upld;
    if (!req || !iter_cb)
        return false;
    if (req->uplds)
        LL_FOREACH(req->uplds, upld) {
            iter_cb(iter_cls, upld);
        }
    return true;
}

#undef _BF_HTTPSRV_REQ_GET_PROP

#undef _BF_HTTPSRV_REQ_GET_VAL

#undef _BF_HTTPSRV_REQ_TRY_VAL

#undef _BF_HTTPSRV_REQ_ITER

#undef _BF_HTTPSRV_REQ_REF

#undef _BF_HTTPSRV_REQ_PAYLOAD
