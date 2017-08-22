#ifdef WIN32
#include <libgen.h>
#else
#include <limits.h>
#endif
#include <sys/stat.h>
#include <microhttpd.h>
#include "b4r_macros.h"
#include "bf_httpsrv_strs.h"
#include <bf_httpsrv_res_cache.h>
#include "bf_httpsrv_req.h"
#include "bf_httpsrv_res.h"
#include "bf_httpsrv.h"

#define _BF_HTTPSRV_RES_RESET(res) /*\
do { \
    ContentFile = '';
    ContentDisposition = '';
    LastModified = 0;
    AcceptRanges = '';
} while (0)*/

#define _BF_HTTPSRV_RES_PRINTF_VA(res, ct, fmt, va) do { \
    utstring_clear(res->body); \
    _BF_HTTPSRV_RES_RESET(res); \
    res->content_type = ct; \
    utstring_printf_va(res->body, fmt, va); \
} while (0)

#define _BF_HTTPSRV_RES_PRINTF(res, ct, fmt, ...) do { \
    va_list va; \
    va_start(va, fmt); \
    _BF_HTTPSRV_RES_PRINTF_VA(res, ct, fmt, va); \
    va_end(va); \
} while (0)

#define _BF_HTTPSRV_RES_GET_BODY(res) (res) ? utstring_body(res->body) : NULL

#define _BF_HTTPSRV_RES_REF(el, mb) (el) ? el->mb : NULL

struct bf_httpsrv_res *_bf_httpsrv_res_new(struct b4r_httpsrv_req *req) {
    struct bf_httpsrv_res *res;
    _B4R_NEW(&res);
    //TODO: headers
    utstring_new(res->body);
    res->req = req;
    res->cache = _bf_httpsrv_res_cache_new(req->headers, res->headers);
    //TODO: range
    res->content_type = req->owner->cfg->content_type;
    res->status_code = MHD_HTTP_OK;
    return res;
}

void _bf_httpsrv_res_free(struct bf_httpsrv_res *res) {
    if (res) {
        free(res->content_file);
        utstring_free(res->body);
        _bf_httpsrv_res_cache_free(res->cache);
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

#define EMPTY_PAGE "{\"status\":\"ERROR\",\"error\":\"Not Found\"}"

int _bf_httpsrv_res_dispatch(struct b4r_httpsrv_req *req) {
    struct MHD_Response *res;
    struct b4r_hs *hs, *hs_tmp;
    size_t max_body_size;
    char *max_body_size_str;
    int ret;

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
            MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_TYPE, BF_HTTPSRV_JSON_CONTENT_TYPE);
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
            //TODO: criar uma macro _B4R_BASENAME()
            sprintf(attach_fn, "attachment; filename=\"%s\"", basename(req->res->content_file));
            MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_DISPOSITION, attach_fn);
            ret = MHD_queue_response(req->con, MHD_HTTP_OK, res);
            MHD_destroy_response(res);
        }
        return ret;
    }


    max_body_size = utstring_len(req->res->body);
    if (req->owner->cfg->max_body_size > 0 && max_body_size > req->owner->cfg->max_body_size) {
        max_body_size_str = b4r_fmt_size(req->owner->cfg->max_body_size);
        _bf_httpsrv_req_err(req, S_BF_MAX_ALLOWED_BODY, max_body_size_str);
        _B4R_FREE(max_body_size_str);
        max_body_size = utstring_len(req->res->body);
        if (max_body_size > req->owner->cfg->max_body_size)
            return MHD_NO;
    }
    res = MHD_create_response_from_buffer(max_body_size, utstring_body(req->res->body), MHD_RESPMEM_MUST_COPY);

    /* TODO: o códdigo abaixo é transitório. */
    MHD_add_response_header(res, MHD_HTTP_HEADER_CONTENT_TYPE, req->res->content_type);
    HASH_ITER(hh, req->res->headers, hs, hs_tmp) {
        MHD_add_response_header(res, hs->name, hs->val);
    }

    ret = MHD_queue_response(req->con, req->res->status_code, res);
    MHD_destroy_response(res);
    return ret;
}

void *bf_httpsrv_res_owner(struct bf_httpsrv_res *res) {
    return res ? res->req->owner : NULL;
}

void *bf_httpsrv_res_headers_ref(struct bf_httpsrv_res *res) {
    return _BF_HTTPSRV_RES_REF(res, headers);
}

bool bf_httpsrv_res_header(struct bf_httpsrv_res *res, const char *name, const char *val) {
    if (b4r_is_empty(name) || b4r_is_empty(val))
        return false;
    b4r_hs_add_or_set(&res->headers, name, val);
    return true;
}

bool bf_httpsrv_res_status(struct bf_httpsrv_res *res, uint16_t code) {
    if (!res || code < 100 || code > 599)
        return false;
    res->status_code = code;
    return false;
}

bool bf_httpsrv_res_content_type(struct bf_httpsrv_res *res, const char *content_type) {
    if (b4r_is_empty(content_type))
        return false;
    res->content_type = content_type;
    return true;
}

bool bf_httpsrv_res_clear(struct bf_httpsrv_res *res) {
    if (!res)
        return false;
    //TODO: headers
    utstring_clear(res->body);
    res->status_code = MHD_HTTP_OK;
    res->content_type = res->req->owner->cfg->content_type;
    _BF_HTTPSRV_RES_RESET(res);
    return true;
}

bool bf_httpsrv_res_write_raw(struct bf_httpsrv_res *res, const void *data, size_t size) {
    if (!res)
        return false;
    utstring_bincpy(res->body, data, size);
    return true;
}

bool bf_httpsrv_res_write_va(struct bf_httpsrv_res *res, const char *fmt, va_list va) {
    if (!res)
        return false;
    utstring_printf_va(res->body, fmt, va);
    return true;
}

bool bf_httpsrv_res_write(struct bf_httpsrv_res *res, const char *fmt, ...) {
    va_list va;
    if (!res)
        return false;
    va_start(va, fmt);
    utstring_printf_va(res->body, fmt, va);
    va_end(va);
    return true;
}

bool bf_httpsrv_res_send_va(struct bf_httpsrv_res *res, const char *fmt, va_list va) {
    if (!res)
        return false;
    _BF_HTTPSRV_RES_PRINTF_VA(res, BF_HTTPSRV_HTML_CONTENT_TYPE, fmt, va);
    return true;
}

bool bf_httpsrv_res_send(struct bf_httpsrv_res *res, const char *fmt, ...) {
    if (!res)
        return false;
    _BF_HTTPSRV_RES_PRINTF(res, BF_HTTPSRV_HTML_CONTENT_TYPE, fmt);
    return true;
}

bool bf_httpsrv_res_send_file(struct bf_httpsrv_res *res, const char *filename) {
    if (!res)
        return false;
    free(res->content_file);
    res->content_file = b4r_dup(filename);
    res->content_type = "application/octet-stream";
    return true;
}

bool bf_httpsrv_res_json(struct bf_httpsrv_res *res, const char *json) {
    if (!res)
        return false;
    utstring_clear(res->body);
    _BF_HTTPSRV_RES_RESET(res);
    res->content_type = BF_HTTPSRV_JSON_CONTENT_TYPE;
    utstring_printf(res->body, "%s", json);
    return true;
}

const char *bf_httpsrv_res_body(struct bf_httpsrv_res *res) {
    return _BF_HTTPSRV_RES_GET_BODY(res);
}

const char *bf_httpsrv_res_read(struct bf_httpsrv_res *res) {
    return _BF_HTTPSRV_RES_GET_BODY(res);
}

#undef _BF_HTTPSRV_RES_RESET

#undef _BF_HTTPSRV_RES_PRINTF_VA

#undef _BF_HTTPSRV_RES_PRINTF

#undef _BF_HTTPSRV_RES_GET_BODY

#undef _BF_HTTPSRV_RES_REF