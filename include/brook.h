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

#ifndef B4R_H
#define B4R_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef B4R_EXTERN
# ifdef B4R_SHARED_LIBRARY
#  ifdef _WIN32
#   define B4R_EXTERN __declspec(dllexport)
#  else
#   define B4R_EXTERN extern
#  endif
# else
#  ifdef B4R_STATIC_LIBRARY
#   define B4R_EXTERN
#  else
#   define B4R_EXTERN extern
#  endif
# endif
#endif

#ifdef __GNUC__

#ifndef __format
# define __format(args...) __attribute__((__format__(__printf__, args)))
#endif

#ifndef __nonnull
# define __nonnull(params)
#endif

#ifndef __variadic
# ifdef _WIN32
#  define __variadic
# else
#  define __variadic __attribute__((__sentinel__))
# endif
#endif

#ifndef __malloc
# ifdef _WIN32
#  define __malloc
# else
#  define __malloc __attribute_malloc__
# endif
#endif

#endif

#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#endif

#define B4R_ERR_SIZE 256

#define B4R_UUID_STR_LEN 36

#define B4R_MD5_SIZE 16

#define B4R_MD5_HASH_SIZE 32

/* common */

struct b4r_hs;

typedef bool (*b4r_uuid_func)(char *uuid);

typedef bool (*b4r_hs_iter_cb)(void *cls, struct b4r_hs *hs);

typedef int (*b4r_hs_sort_cb)(void *cls, struct b4r_hs *a, struct b4r_hs *b);

B4R_EXTERN const char *b4r_hs_name(struct b4r_hs *hs) __nonnull((1));

B4R_EXTERN const char *b4r_hs_val(struct b4r_hs *hs) __nonnull((1));

B4R_EXTERN bool b4r_hs_name_val(struct b4r_hs *hs, const char **name, const char **val) __nonnull((1));

B4R_EXTERN bool b4r_hs_add(struct b4r_hs **hsl, const char *name, const char *val) __nonnull((1, 2));

B4R_EXTERN bool b4r_hs_add_or_set(struct b4r_hs **hsl, const char *name, const char *val) __nonnull((1, 2));

B4R_EXTERN bool b4r_hs_rm(struct b4r_hs **hsl, const char *name) __nonnull((1, 2));

B4R_EXTERN struct b4r_hs *b4r_hs_find(struct b4r_hs *hsl, const char *name) __nonnull((1, 2));

B4R_EXTERN const char *b4r_hs_find_val(struct b4r_hs *hsl, const char *name) __nonnull((1, 2));

B4R_EXTERN bool b4r_hs_try(struct b4r_hs *hsl, const char *name, const char **val) __nonnull((1, 2));

B4R_EXTERN bool b4r_hs_has(struct b4r_hs *hsl, const char *name) __nonnull((1, 2));

B4R_EXTERN bool b4r_hs_iter(struct b4r_hs *hsl, b4r_hs_iter_cb iter_cb, void *iter_cls) __nonnull((1, 2));

//TODO: warn in doc that can not use b4r_hs_next() in deleting items.
B4R_EXTERN bool b4r_hs_next(struct b4r_hs **hs) __nonnull((1));

B4R_EXTERN unsigned int b4r_hs_count(struct b4r_hs *hsl) __nonnull((1));

B4R_EXTERN bool b4r_hs_sort(struct b4r_hs **hsl, b4r_hs_sort_cb cmp_cb, void *cmp_cls) __nonnull((1, 2));

B4R_EXTERN void b4r_hs_cleanup(struct b4r_hs **hsl) __nonnull((1));

/* server */

#define B4R_HTTPSRV_PORT 8080

#if defined(__ANDROID__)
# define B4R_HTTPSRV_POST_BUF_SIZE 1024 /* 1 kB */
#elif defined(WIN32)
# define B4R_HTTPSRV_POST_BUF_SIZE 32768 /* 32 kB */
#else
# define B4R_HTTPSRV_POST_BUF_SIZE 65536 /* 64 kB */
#endif

#if defined(__ANDROID__)

#define B4R_HTTPSRV_MAX_BODY_SIZE 524288 /* 512 kB */

#define B4R_HTTPSRV_MAX_PAYLD_SIZE 524288 /* 512 kB */

#define B4R_HTTPSRV_MAX_UPLD_SIZE 4194304 /* 4 MB */

#else

#define B4R_HTTPSRV_MAX_BODY_SIZE 2097152 /* 2 MB */

#define B4R_HTTPSRV_MAX_PAYLD_SIZE 2097152 /* 2 MB */

#define B4R_HTTPSRV_MAX_UPLD_SIZE 15728640 /* 15 MB */

#endif

#define B4R_HTTPSRV_HTML_CONTENT_TYPE "text/html; charset=UTF-8"

#define B4R_HTTPSRV_JSON_CONTENT_TYPE "application/json; charset=UTF-8"

#define B4R_HTTPSRV_CFG_PORT "b4r_httpsrv_cfg_port"

#define B4R_HTTPSRV_CFG_UPLDS_DIR "b4r_httpsrv_cfg_upld_dir"

#define B4R_HTTPSRV_CFG_MAX_BODY_SIZE "b4r_httpsrv_cfg_max_body_size"

#define B4R_HTTPSRV_CFG_POST_BUF_SIZE "b4r_httpsrv_cfg_post_buf_size"

#define B4R_HTTPSRV_CFG_MAX_PAYLD_SIZE "b4r_httpsrv_cfg_max_payld_size"

#define B4R_HTTPSRV_CFG_MAX_UPLD_SIZE "b4r_httpsrv_cfg_max_upld_size"

#define B4R_HTTPSRV_CFG_CONTENT_TYPE "b4r_httpsrv_cfg_content_type"

#define B4R_HTTPSRV_CFG_UUID_FUNC "b4r_httpsrv_cfg_uuid_func"

enum B4R_HTTPSRV_AUTH_TYPE {
    B4R_HTTPSRV_AUTH_TYPE_BASIC = 1,
    B4R_HTTPSRV_AUTH_TYPE_DIGEST = 2
};

enum B4R_HTTPSRV_OPT {
    B4R_HTTPSRV_OPT_CON_CB = 0,
    B4R_HTTPSRV_OPT_REQ_INIT_CB = 1,
    B4R_HTTPSRV_OPT_REQ_FINI_CB = 2,
    B4R_HTTPSRV_OPT_REQ_PREP_CB = 3,
    B4R_HTTPSRV_OPT_REQ_UPLD_DATA_CB = 4,
    B4R_HTTPSRV_OPT_REQ_UPLD_FILE_CBS = 5,
    B4R_HTTPSRV_OPT_REQ_ERR_CB = 6,
    B4R_HTTPSRV_OPT_REQ_ALLOWED_POST = 7,
    B4R_HTTPSRV_OPT_ERR_CB = 8,
    B4R_HTTPSRV_OPT_PORT = 9,
    B4R_HTTPSRV_OPT_SHUTDOWN_ATTEMPTS = 10,
    B4R_HTTPSRV_OPT_FORCED_SHUTDOWN = 11,
    B4R_HTTPSRV_OPT_AUTH = 12,
    B4R_HTTPSRV_OPT_ENABLED_LOG = 13
};

struct b4r_httpsrv_cfg;

struct b4r_httpsrv;

struct b4r_httpsrv_req;

struct b4r_httpsrv_res;

struct b4r_httpsrv_req_upld;

typedef void (*b4r_httpsrv_con_cb)(void *cls, const char *id, bool closed);

typedef void (*b4r_httpsrv_req_init_cb)(void *cls, const char *id, const char *uri, bool *aborted);

typedef void (*b4r_httpsrv_req_fini_cb)(void *cls, bool finished);

typedef void (*b4r_httpsrv_req_cb)(void *cls, struct b4r_httpsrv_req *req, struct b4r_httpsrv_res *res, bool *done);

typedef void (*b4r_httpsrv_req_prep_cb)(void *cls, struct b4r_httpsrv_req *req, struct b4r_httpsrv_res *res, bool *done);

typedef bool (*b4r_httpsrv_req_upld_data_cb)(void *cls, struct b4r_httpsrv_req *req, const char *buf, size_t size,
                                             char *err);

typedef void *(*b4r_httpsrv_req_upld_file_prepare_cb)(void *cls, struct b4r_httpsrv_req_upld *upld);

typedef bool (*b4r_httpsrv_req_upld_file_save_cb)(void *cls, struct b4r_httpsrv_req_upld *upld, bool overwritten);

typedef bool (*b4r_httpsrv_req_upld_file_write_cb)(void *cls, struct b4r_httpsrv_req_upld *upld,
                                                   const char *buf, size_t size);

typedef void (*b4r_httpsrv_req_err_cb)(void *cls, struct b4r_httpsrv_req *req, struct b4r_httpsrv_res *res, bool *done,
                                       const char *fmt, va_list va);

typedef void (*b4r_httpsrv_err_cb)(void *cls, const char *fmt, va_list va);

typedef void (*b4r_httpsrv_req_uplds_iter_cb)(void *cls, struct b4r_httpsrv_req_upld *upld);

//TODO: document the following:
// - if you pass the file name, Brook loads the config from that;
// - if you do not pass the file name, Brook tries to load config from envvars;
// - if Brook does not find any config of these above, loads its defaults.
B4R_EXTERN struct b4r_httpsrv_cfg *b4r_httpsrv_cfg_new(const char *filename);

B4R_EXTERN void b4r_httpsrv_cfg_free(struct b4r_httpsrv_cfg *cfg) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_cfg_set_va(struct b4r_httpsrv_cfg *cfg, const char *name, va_list va) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_cfg_set(struct b4r_httpsrv_cfg *cfg, const char *name, ...) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_cfg_get(struct b4r_httpsrv_cfg *cfg, const char *name, ...) __nonnull((1, 2));

B4R_EXTERN struct b4r_httpsrv *b4r_httpsrv_new(struct b4r_httpsrv_cfg *cfg, b4r_httpsrv_req_cb req_cb, void *req_cls,
                                               b4r_httpsrv_req_err_cb req_err_cb, void *req_err_cls,
                                               b4r_httpsrv_err_cb err_cb, void *err_cls) __nonnull((1, 2));

B4R_EXTERN void b4r_httpsrv_free(struct b4r_httpsrv *srv) __nonnull((1));

//TODO: add functions to get the cfg instance.

B4R_EXTERN bool b4r_httpsrv_listen(struct b4r_httpsrv *srv) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_listening(struct b4r_httpsrv *srv) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_shutdown(struct b4r_httpsrv *srv) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_setopt(struct b4r_httpsrv *srv, enum B4R_HTTPSRV_OPT opt, ...) __nonnull((1));

B4R_EXTERN unsigned int b4r_httpsrv_active_conns(struct b4r_httpsrv *srv) __nonnull((1));

B4R_EXTERN void *b4r_httpsrv_req_owner(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_version(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_id(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_uri(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_path(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_method(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_content_type(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN size_t b4r_httpsrv_req_content_len(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_req_up(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_header(struct b4r_httpsrv_req *req, const char *name) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_try_header(struct b4r_httpsrv_req *req, const char *name,
                                         const char **val) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_iter_headers(struct b4r_httpsrv_req *req,
                                           b4r_hs_iter_cb iter_cb, void *iter_cls) __nonnull((1, 2));

B4R_EXTERN void *b4r_httpsrv_req_headers_ref(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_cookie(struct b4r_httpsrv_req *req, const char *name) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_try_cookie(struct b4r_httpsrv_req *req, const char *name,
                                           const char **val) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_iter_cookies(struct b4r_httpsrv_req *req,
                                             b4r_hs_iter_cb iter_cb, void *iter_cls) __nonnull((1, 2));

B4R_EXTERN void *b4r_httpsrv_req_cookies_ref(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_param(struct b4r_httpsrv_req *req, const char *name) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_try_param(struct b4r_httpsrv_req *req, const char *name,
                                          const char **val) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_iter_params(struct b4r_httpsrv_req *req,
                                            b4r_hs_iter_cb iter_cb, void *iter_cls) __nonnull((1, 2));

B4R_EXTERN void *b4r_httpsrv_req_params_ref(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_field(struct b4r_httpsrv_req *req, const char *name) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_try_field(struct b4r_httpsrv_req *req, const char *name,
                                          const char **val) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_iter_fields(struct b4r_httpsrv_req *req,
                                            b4r_hs_iter_cb iter_cb, void *iter_cls) __nonnull((1, 2));

B4R_EXTERN void *b4r_httpsrv_req_fields_ref(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_payld(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_req_write(struct b4r_httpsrv_req *req, const char *buf, size_t size) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_send(struct b4r_httpsrv_req *req, const char *buf) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_iter_uplds(struct b4r_httpsrv_req *req,
                                          b4r_httpsrv_req_uplds_iter_cb iter_cb, void *iter_cls) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_req_uplds_first(struct b4r_httpsrv_req *req,
                                            struct b4r_httpsrv_req_upld **upld) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_uplds_next(struct b4r_httpsrv_req_upld **upld) __nonnull((1));

B4R_EXTERN int b4r_httpsrv_req_uplds_count(struct b4r_httpsrv_req *req) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_req_upld_save(struct b4r_httpsrv_req *req, struct b4r_httpsrv_req_upld *upld,
                                          bool overwritten) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_req_upld_save_as(struct b4r_httpsrv_req *req, struct b4r_httpsrv_req_upld *upld,
                                             const char *name, bool overwritten) __nonnull((1, 2));

B4R_EXTERN void *b4r_httpsrv_req_upld_stream(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN uint64_t b4r_httpsrv_req_upld_size(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_name(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_orig_name(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_dir(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_dest_name(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_field(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_mime(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_encoding(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN const char *b4r_httpsrv_req_upld_err(struct b4r_httpsrv_req_upld *upld) __nonnull((1));

B4R_EXTERN int b4r_httpsrv_req_upld_failf_va(struct b4r_httpsrv_req_upld *upld, const char *fmt,
                                             va_list va) __nonnull((1, 2));

B4R_EXTERN int b4r_httpsrv_req_upld_failf(struct b4r_httpsrv_req_upld *upld,
                                          const char *fmt, ...) __nonnull((1, 2)) __format(2, 3);

B4R_EXTERN void *b4r_httpsrv_res_owner(struct b4r_httpsrv_res *res) __nonnull((1));

B4R_EXTERN void *b4r_httpsrv_res_headers_ref(struct b4r_httpsrv_res *res) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_res_header(struct b4r_httpsrv_res *res, const char *name,
                                       const char *val) __nonnull((1, 2, 3));

B4R_EXTERN bool b4r_httpsrv_res_status(struct b4r_httpsrv_res *res, uint16_t code) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_res_content_type(struct b4r_httpsrv_res *res, const char *content_type) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_res_clear(struct b4r_httpsrv_res *res) __nonnull((1));

B4R_EXTERN bool b4r_httpsrv_res_write_raw(struct b4r_httpsrv_res *res, const void *data, size_t size) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_res_write_va(struct b4r_httpsrv_res *res, const char *fmt, va_list va) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_res_write(struct b4r_httpsrv_res *res, const char *fmt, ...) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_res_send_va(struct b4r_httpsrv_res *res, const char *fmt, va_list va) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_res_send(struct b4r_httpsrv_res *res, const char *fmt, ...) __nonnull((1, 2)) __format(2, 3);

B4R_EXTERN bool b4r_httpsrv_res_send_file(struct b4r_httpsrv_res *res, const char *filename) __nonnull((1, 2));

B4R_EXTERN bool b4r_httpsrv_res_json(struct b4r_httpsrv_res *res, const char *json) __nonnull((1, 2));

B4R_EXTERN const char *b4r_httpsrv_res_body(struct b4r_httpsrv_res *res) __nonnull((1));

//TODO: write in the doc that this function is just an alias for b4r_httpsrv_res_body() one
B4R_EXTERN const char *b4r_httpsrv_res_read(struct b4r_httpsrv_res *res) __nonnull((1));

/* router */

typedef void (*b4r_router_cb)(void *cls, void *req, void *res);

B4R_EXTERN struct b4r_router *b4r_router_new();

B4R_EXTERN void b4r_router_free(struct b4r_router *rtr) __nonnull((1));

B4R_EXTERN void b4r_router_route(struct b4r_router *rtr, void *req, void *res) __nonnull((1, 2, 3));

//TODO: add callback register a route as "service"
B4R_EXTERN void b4r_router_reg_sv_cb(b4r_router_cb cb, void *cls) __nonnull((1));

//TODO: add callback register a route as "request"
B4R_EXTERN void b4r_router_reg_req_cb(b4r_router_cb cb, void *cls) __nonnull((1));

//TODO: add function to register a route with a function to call a create and a destroy handle, hence it will make sense to have the flag B4R_ROUTER_SCOPE

/* utils */

B4R_EXTERN void *b4r_alloc(size_t size) __malloc;

B4R_EXTERN void b4r_free(void *ptr);

B4R_EXTERN char *b4r_dup(const char *str);

B4R_EXTERN bool b4r_is_empty(const char *str) __nonnull((1));

B4R_EXTERN size_t b4r_index(const char *str, const char *substr) __nonnull((1, 2));

//TODO: document that despite not having const it does not allocate anything internally, the trim is done inline!
B4R_EXTERN char *b4r_lower(char *str) __nonnull((1)) __malloc;

//TODO: document that despite not having const it does not allocate anything internally, the trim is done inline!
B4R_EXTERN char *b4r_upper(char *str) __nonnull((1)) __malloc;

//TODO: document that despite not having const it does not allocate anything internally, the trim is done inline!
B4R_EXTERN char *b4r_ltrim(char *str) __nonnull((1)) __malloc;

//TODO: document that despite not having const it does not allocate anything internally, the trim is done inline!
B4R_EXTERN char *b4r_rtrim(char *str) __nonnull((1)) __malloc;

//TODO: document that despite not having const it does not allocate anything internally, the trim is done inline!
#define B4R_TRIM(str) b4r_rtrim(b4r_ltrim(str))

B4R_EXTERN char *b4r_trim(char *str) __nonnull((1)) __malloc;

B4R_EXTERN char *b4r_cat(const char *str1, const char *str2) __nonnull((1, 2)) __malloc;

B4R_EXTERN char *b4r_concat(const char *str1, ...) __nonnull((1)) __variadic __malloc;

B4R_EXTERN bool b4r_has_suffix(const char *str, const char *suf) __nonnull((1, 2));

B4R_EXTERN bool b4r_has_prefix(const char *str, const char *pre) __nonnull((1, 2));

B4R_EXTERN size_t b4r_str_to_size(const char *str) __nonnull((1));

B4R_EXTERN char *b4r_asprintf_va(const char *fmt, va_list va) __nonnull((1, 2));

B4R_EXTERN char *b4r_asprintf(const char *fmt, ...) __nonnull((1));

B4R_EXTERN bool b4r_is_post(const char *method) __nonnull((1));

B4R_EXTERN char *b4r_add_path_suffix(const char *path) __nonnull((1));

B4R_EXTERN char *b4r_add_path_prefix(const char *path) __nonnull((1));

B4R_EXTERN const char *b4r_tmp_dir();

B4R_EXTERN char *b4r_fmt_size(uint64_t size);

B4R_EXTERN bool b4r_uuid(char *uuid);

B4R_EXTERN bool b4r_md5(const void *buf, unsigned long size, char *hash) __nonnull((1, 3));

B4R_EXTERN bool b4r_md5_hex(const void *buf, char *hash) __nonnull((1, 2));

#ifdef __cplusplus
}
#endif

#endif