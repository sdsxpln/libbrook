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
#include "b4r_macros.h"
#include "b4r_utils.h"
#include "b4r_httpsrv_cfg.h"

struct b4r_httpsrv_cfg *b4r_httpsrv_cfg_new(const char *filename) {
    struct b4r_httpsrv_cfg *cfg;
    _B4R_NEW(&cfg);
    cfg->filename = b4r_dup(filename);

    //TODO: load values below only if it can not be read from file or envvar.
    cfg->uplds_dir = b4r_dup(b4r_tmp_dir());
    cfg->port = B4R_HTTPSRV_PORT;
    cfg->max_body_size = B4R_HTTPSRV_MAX_BODY_SIZE;
    cfg->post_buffer_size = B4R_HTTPSRV_POST_BUF_SIZE;
    cfg->max_payld_size = B4R_HTTPSRV_MAX_PAYLD_SIZE;
    cfg->max_upld_size = B4R_HTTPSRV_MAX_UPLD_SIZE;
    cfg->content_type = b4r_dup(B4R_HTTPSRV_HTML_CONTENT_TYPE);
    cfg->uuid_func = b4r_uuid;

    return cfg;
}

void b4r_httpsrv_cfg_free(struct b4r_httpsrv_cfg *cfg) {
    if (cfg) {
        _B4R_FREE(cfg->filename);
        _B4R_FREE(cfg->content_type);
        _B4R_FREE(cfg->uplds_dir);
        _B4R_FREE(cfg);
    }
}

bool b4r_httpsrv_cfg_set_va(struct b4r_httpsrv_cfg *cfg, const char *name, va_list va) {
    bool ret;
    if (!cfg || b4r_is_empty(name))
        return false;
#define IS_OPT(op) strcmp(name, op) == 0
#define SET_ORD_OPT(mb, ex, def, tp) do { \
    cfg->mb = va_arg(va, tp); \
    ret = (ex); \
    if (!ret) \
        cfg->mb = def; \
} while (0)
#define SET_STR_OPT(mb, ex, def, tp) do { \
    _B4R_FREE(cfg->mb); \
    char *s = va_arg(va, tp); \
    cfg->mb = b4r_dup(s); \
    ret = (ex); \
    if (!ret) \
        cfg->mb = b4r_dup(def); \
} while (0)
    if (IS_OPT(B4R_HTTPSRV_CFG_PORT))
        SET_ORD_OPT(port, cfg->port > 0, B4R_HTTPSRV_PORT, unsigned int);
    else if (IS_OPT(B4R_HTTPSRV_CFG_UPLDS_DIR)) {
        SET_STR_OPT(uplds_dir, !b4r_is_empty(cfg->uplds_dir), b4r_tmp_dir(), char *);
    } else if (IS_OPT(B4R_HTTPSRV_CFG_POST_BUF_SIZE))
        SET_ORD_OPT(post_buffer_size, cfg->post_buffer_size >= 0, B4R_HTTPSRV_POST_BUF_SIZE, size_t);
    else if (IS_OPT(B4R_HTTPSRV_CFG_MAX_BODY_SIZE))
        SET_ORD_OPT(max_body_size, cfg->max_body_size >= 0, B4R_HTTPSRV_MAX_BODY_SIZE, unsigned long);
    else if (IS_OPT(B4R_HTTPSRV_CFG_MAX_PAYLD_SIZE))
        SET_ORD_OPT(max_payld_size, cfg->max_payld_size >= 0, B4R_HTTPSRV_MAX_PAYLD_SIZE, unsigned long);
    else if (IS_OPT(B4R_HTTPSRV_CFG_MAX_UPLD_SIZE))
        SET_ORD_OPT(max_upld_size, cfg->max_upld_size >= 0, B4R_HTTPSRV_MAX_UPLD_SIZE, uint64_t);
    else if (IS_OPT(B4R_HTTPSRV_CFG_CONTENT_TYPE))
        SET_STR_OPT(content_type, !b4r_is_empty(cfg->content_type), B4R_HTTPSRV_HTML_CONTENT_TYPE, char *);
    else if (IS_OPT(B4R_HTTPSRV_CFG_UUID_FUNC)) {
        SET_ORD_OPT(uuid_func, cfg->uuid_func, b4r_uuid, b4r_uuid_func);
        if (!cfg->uuid_func)
            cfg->uuid_func = b4r_uuid;
#undef IS_OPT
    }
#undef SET_ORD_OPT
#undef SET_STR_OPT
    else
        ret = false;
    return ret;
}

bool b4r_httpsrv_cfg_set(struct b4r_httpsrv_cfg *cfg, const char *name, ...) {
    va_list va;
    bool ret;
    if (!cfg)
        return false;
    va_start(va, name);
    ret = b4r_httpsrv_cfg_set_va(cfg, name, va);
    va_end(va);
    return ret;
}

bool b4r_httpsrv_cfg_get(struct b4r_httpsrv_cfg *cfg, const char *name, ...) {
    va_list va;
    bool ret;
    if (!cfg || b4r_is_empty(name))
        return false;
    va_start(va, name);
#define IS_OPT(op) strcmp(name, op) == 0
#define GET_OPT(mb, tp) do { \
    tp *val = va_arg(va, tp*); \
    ret = (val); \
    if (ret) \
        *val = cfg->mb; \
} while (0)
    if (IS_OPT(B4R_HTTPSRV_CFG_PORT))
        GET_OPT(port, uint16_t);
    else if (IS_OPT(B4R_HTTPSRV_CFG_UPLDS_DIR))
        GET_OPT(uplds_dir, char *);
    else if (IS_OPT(B4R_HTTPSRV_CFG_POST_BUF_SIZE))
        GET_OPT(post_buffer_size, size_t);
    else if (IS_OPT(B4R_HTTPSRV_CFG_MAX_BODY_SIZE))
        GET_OPT(max_body_size, unsigned long);
    else if (IS_OPT(B4R_HTTPSRV_CFG_MAX_PAYLD_SIZE))
        GET_OPT(max_payld_size, unsigned long);
    else if (IS_OPT(B4R_HTTPSRV_CFG_MAX_UPLD_SIZE))
        GET_OPT(max_upld_size, uint64_t);
    else if (IS_OPT(B4R_HTTPSRV_CFG_CONTENT_TYPE))
        GET_OPT(content_type, char *);
    else if (IS_OPT(B4R_HTTPSRV_CFG_UUID_FUNC))
        GET_OPT(uuid_func, b4r_uuid_func);
#undef IS_OPT
#undef GET_OPT
    else
        ret = false;
    va_end(va);
    return ret;
}