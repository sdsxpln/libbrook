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

#include <libgen.h>
#include <sys/stat.h>
#include <microhttpd.h>
#include "b4r_macros.h"
#ifdef UNIX
#include <limits.h>
#endif
#include "b4r_httpsrv_strs.h"
#include <b4r_httpsrv_res_cache.h>
#include "b4r_httpsrv_req.h"
#include "b4r_httpsrv_res.h"
#include "b4r_httpsrv.h"

#define _B4R_HTTPSRV_RES_RESET(res) /*\
do { \
    ContentFile = '';
    ContentDisposition = '';
    LastModified = 0;
    AcceptRanges = '';
} while (0)*/

#define _B4R_HTTPSRV_RES_PRINTF_VA(res, ct, fmt, va) do { \
    utstring_clear((res)->body); \
    _B4R_HTTPSRV_RES_RESET(res); \
    (res)->content_type = ct; \
    utstring_printf_va((res)->body, fmt, va); \
} while (0)

#define _B4R_HTTPSRV_RES_PRINTF(res, ct, fmt, ...) do { \
    va_list va; \
    va_start(va, fmt); \
    _B4R_HTTPSRV_RES_PRINTF_VA(res, ct, fmt, va); \
    va_end(va); \
} while (0)

struct b4r_httpsrv_res *_b4r_httpsrv_res_new(struct b4r_httpsrv_req *req) {
    struct b4r_httpsrv_res *res;
    _B4R_NEW(&res);
    //TODO: headers
    utstring_new(res->body);
    res->req = req;
    res->cache = _b4r_httpsrv_res_cache_new(req->headers, res->headers);
    //TODO: range
    res->content_type = req->owner->cfg->content_type;
    res->status_code = MHD_HTTP_OK;
    return res;
}

void _b4r_httpsrv_res_free(struct b4r_httpsrv_res *res) {
    if (res) {
        free(res->content_file);
        utstring_free(res->body);
        _b4r_httpsrv_res_cache_free(res->cache);
        _B4R_FREE(res);
    }
}

static ssize_t test_file_reader(void *cls, uint64_t pos, char *buf, size_t max) {
    FILE *file = cls;
    fseek(file, pos, SEEK_SET);
    return fread(buf, 1, max, file);
}

static void test_file_free_callback(void *cls) {
    FILE *file = cls;
    fclose(file);
}

/* TODO: the macro below is transitory. */
#define EMPTY_PAGE "{\"status\":\"ERROR\",\"error\":\"Not Found\"}"

int _b4r_httpsrv_res_dispatch(struct b4r_httpsrv_req *req) {
    struct MHD_Response *res;
    struct b4r_hs *hs, *hs_tmp;
    size_t body_size;
    char *body_size_str;
    char err[B4R_ERR_SIZE];
    int ret;

    /* TODO: the whole code below is transitory. */

    req->owner->req_cb(req->owner->req_cls, req, req->res, &req->done);

    if (!req->done)
        return MHD_NO;

    if (!b4r_is_empty(req->res->content_file)) {
        FILE *file;
        int fd;
        struct stat buf;
        file = fopen(req->res->content_file, "rb");
        if (file) {
            fd = fileno(file);
            if (fd == -1) {
                fclose(file);
                return MHD_NO;
            }
            if ((fstat(fd, &buf) != 0) ||
                (!S_ISREG(buf.st_mode))) {
                fclose(file);
                file = NULL;
            }
        }
        if (!file) {
            res = MHD_create_response_from_buffer(strlen(EMPTY_PAGE), (void *) EMPTY_PAGE, MHD_RESPMEM_PERSISTENT);
            MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_TYPE, B4R_HTTPSRV_JSON_CONTENT_TYPE);
            ret = MHD_queue_response(req->con, MHD_HTTP_NOT_FOUND, res);
            MHD_destroy_response(res);
        } else {
            res = MHD_create_response_from_callback((uint64_t) buf.st_size, 32 * 1024, &test_file_reader, file,
                                                    &test_file_free_callback);
            if (!res) {
                fclose(file);
                return MHD_NO;
            }
            char attach_fn[PATH_MAX];
            //TODO: create _B4R_BASENAME() macro.
            sprintf(attach_fn, "attachment; filename=\"%s\"", basename(req->res->content_file));
            MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_DISPOSITION, attach_fn);
            ret = MHD_queue_response(req->con, MHD_HTTP_OK, res);
            MHD_destroy_response(res);
        }
        return ret;
    }

    body_size = utstring_len(req->res->body);
    if (req->owner->cfg->max_body_size > 0 && body_size > req->owner->cfg->max_body_size &&
        (body_size_str = b4r_fmt_size(req->owner->cfg->max_body_size))) {
        sprintf(err, S_B4R_MAX_ALLOWED_BODY, body_size_str);
        _B4R_FREE(body_size_str);
        _b4r_httpsrv_req_err(req, err);
        body_size = utstring_len(req->res->body);
        if (body_size > req->owner->cfg->max_body_size) {
            if (req->owner->err_cb)
                req->owner->err_cb(req->owner->err_cls, err);
            return MHD_NO;
        }
    }
    res = MHD_create_response_from_buffer(body_size, utstring_body(req->res->body), MHD_RESPMEM_MUST_COPY);

    MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_TYPE, req->res->content_type);
    HASH_ITER(hh, req->res->headers, hs, hs_tmp) {
        MHD_add_response_header(res, hs->name, hs->val);
    }

    ret = MHD_queue_response(req->con, req->res->status_code, res);
    MHD_destroy_response(res);
    return ret;
}

void *b4r_httpsrv_res_owner(struct b4r_httpsrv_res *res) {
    return res ? res->req->owner : NULL;
}

struct b4r_hs **b4r_httpsrv_res_headers(struct b4r_httpsrv_res *res) {
    return res ? &res->headers : NULL;
}

bool b4r_httpsrv_res_header(struct b4r_httpsrv_res *res, const char *name, const char *val) {
    if (b4r_is_empty(name) || b4r_is_empty(val))
        return false;
    b4r_hs_add_or_set(&res->headers, name, val);
    return true;
}

bool b4r_httpsrv_res_status(struct b4r_httpsrv_res *res, uint16_t code) {
    if (!res || code < 100 || code > 599)
        return false;
    res->status_code = code;
    return false;
}

bool b4r_httpsrv_res_content_type(struct b4r_httpsrv_res *res, const char *content_type) {
    if (b4r_is_empty(content_type))
        return false;
    res->content_type = content_type;
    return true;
}

bool b4r_httpsrv_res_clear(struct b4r_httpsrv_res *res) {
    if (!res)
        return false;
    //TODO: headers
    utstring_clear(res->body);
    res->status_code = MHD_HTTP_OK;
    res->content_type = res->req->owner->cfg->content_type;
    _B4R_HTTPSRV_RES_RESET(res);
    return true;
}

bool b4r_httpsrv_res_write_raw(struct b4r_httpsrv_res *res, const void *data, size_t size) {
    if (!res)
        return false;
    utstring_bincpy(res->body, data, size);
    return true;
}

bool b4r_httpsrv_res_write_va(struct b4r_httpsrv_res *res, const char *fmt, va_list va) {
    if (!res)
        return false;
    utstring_printf_va(res->body, fmt, va);
    return true;
}

bool b4r_httpsrv_res_write(struct b4r_httpsrv_res *res, const char *fmt, ...) {
    va_list va;
    if (!res)
        return false;
    va_start(va, fmt);
    utstring_printf_va(res->body, fmt, va);
    va_end(va);
    return true;
}

bool b4r_httpsrv_res_send_va(struct b4r_httpsrv_res *res, const char *fmt, va_list va) {
    if (!res)
        return false;
    _B4R_HTTPSRV_RES_PRINTF_VA(res, B4R_HTTPSRV_HTML_CONTENT_TYPE, fmt, va);
    return true;
}

bool b4r_httpsrv_res_send(struct b4r_httpsrv_res *res, const char *fmt, ...) {
    if (!res)
        return false;
    _B4R_HTTPSRV_RES_PRINTF(res, B4R_HTTPSRV_HTML_CONTENT_TYPE, fmt);
    return true;
}

bool b4r_httpsrv_res_send_file(struct b4r_httpsrv_res *res, const char *filename) {
    if (!res)
        return false;
    free(res->content_file);
    res->content_file = b4r_dup(filename);
    res->content_type = "application/octet-stream";
    return true;
}

bool b4r_httpsrv_res_json(struct b4r_httpsrv_res *res, const char *json) {
    if (!res)
        return false;
    utstring_clear(res->body);
    _B4R_HTTPSRV_RES_RESET(res);
    res->content_type = B4R_HTTPSRV_JSON_CONTENT_TYPE;
    utstring_printf(res->body, "%s", json);
    return true;
}

const char *b4r_httpsrv_res_body(struct b4r_httpsrv_res *res) {
    return res ? utstring_body(res->body) : NULL;
}

#undef _B4R_HTTPSRV_RES_RESET

#undef _B4R_HTTPSRV_RES_PRINTF_VA

#undef _B4R_HTTPSRV_RES_PRINTF