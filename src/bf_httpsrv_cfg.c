#include <stdlib.h>
#include <string.h>
#include "b4r_macros.h"
#include "b4r_utils.h"
#include "bf_httpsrv_cfg.h"

struct bf_httpsrv_cfg *bf_httpsrv_cfg_new(const char *filename) {
    struct bf_httpsrv_cfg *cfg;
    _B4R_NEW(&cfg);
    cfg->filename = b4r_dup(filename);

    //TODO: carregar os valores abaixo somente se não conseguir ler de arquivo ou envvar.
    cfg->uplds_dir = b4r_dup(b4r_tmp_dir());
    cfg->port = BF_HTTPSRV_PORT;
    cfg->max_body_size = BF_HTTPSRV_MAX_BODY_SIZE;
    cfg->post_buffer_size = BF_HTTPSRV_POST_BUF_SIZE;
    cfg->max_payld_size = BF_HTTPSRV_MAX_PAYLD_SIZE;
    cfg->max_upld_size = BF_HTTPSRV_MAX_UPLD_SIZE;
    cfg->content_type = b4r_dup(BF_HTTPSRV_HTML_CONTENT_TYPE);
    cfg->uuid_func = b4r_uuid;

    return cfg;
}

void bf_httpsrv_cfg_free(struct bf_httpsrv_cfg *cfg) {
    if (cfg) {
        _B4R_FREE(cfg->filename);
        _B4R_FREE(cfg->content_type);
        _B4R_FREE(cfg->uplds_dir);
        _B4R_FREE(cfg);
    }
}

bool bf_httpsrv_cfg_set_va(struct bf_httpsrv_cfg *cfg, const char *name, va_list va) {
    bool ret;
    if (!cfg || b4r_is_empty(name))
        return false;
#define _IS_OPT(op) strcmp(name, op) == 0
#define _SET_ORD_OPT(mb, ex, def, tp) do { \
    cfg->mb = va_arg(va, tp); \
    ret = (ex); \
    if (!ret) \
        cfg->mb = def; \
} while (0)
#define _SET_STR_OPT(mb, ex, def, tp) do { \
    _B4R_FREE(cfg->mb); \
    char *s = va_arg(va, tp); \
    cfg->mb = b4r_dup(s); \
    ret = (ex); \
    if (!ret) \
        cfg->mb = b4r_dup(def); \
} while (0)
    if (_IS_OPT(BF_HTTPSRV_CFG_PORT))
        _SET_ORD_OPT(port, cfg->port > 0, BF_HTTPSRV_PORT, unsigned int);
    else if (_IS_OPT(BF_HTTPSRV_CFG_UPLDS_DIR)) {
        _SET_STR_OPT(uplds_dir, !b4r_is_empty(cfg->uplds_dir), b4r_tmp_dir(), char *);
    } else if (_IS_OPT(BF_HTTPSRV_CFG_POST_BUF_SIZE))
        _SET_ORD_OPT(post_buffer_size, cfg->post_buffer_size >= 0, BF_HTTPSRV_POST_BUF_SIZE, size_t);
    else if (_IS_OPT(BF_HTTPSRV_CFG_MAX_BODY_SIZE))
        _SET_ORD_OPT(max_body_size, cfg->max_body_size >= 0, BF_HTTPSRV_MAX_BODY_SIZE, unsigned long);
    else if (_IS_OPT(BF_HTTPSRV_CFG_MAX_PAYLD_SIZE))
        _SET_ORD_OPT(max_payld_size, cfg->max_payld_size >= 0, BF_HTTPSRV_MAX_PAYLD_SIZE, unsigned long);
    else if (_IS_OPT(BF_HTTPSRV_CFG_MAX_UPLD_SIZE))
        _SET_ORD_OPT(max_upld_size, cfg->max_upld_size >= 0, BF_HTTPSRV_MAX_UPLD_SIZE, uint64_t);
    else if (_IS_OPT(BF_HTTPSRV_CFG_CONTENT_TYPE))
        _SET_STR_OPT(content_type, !b4r_is_empty(cfg->content_type), BF_HTTPSRV_HTML_CONTENT_TYPE, char *);
    else if (_IS_OPT(BF_HTTPSRV_CFG_UUID_FUNC)) {
        _SET_ORD_OPT(uuid_func, cfg->uuid_func, b4r_uuid, bf_uuid_func);
        if (!cfg->uuid_func)
            cfg->uuid_func = b4r_uuid;
#undef _IS_OPT
    }
#undef _SET_ORD_OPT
#undef _SET_STR_OPT
    else
        ret = false;
    return ret;
}

bool bf_httpsrv_cfg_set(struct bf_httpsrv_cfg *cfg, const char *name, ...) {
    va_list va;
    bool ret;
    if (!cfg)
        return false;
    va_start(va, name);
    ret = bf_httpsrv_cfg_set_va(cfg, name, va);
    va_end(va);
    return ret;
}

bool bf_httpsrv_cfg_get(struct bf_httpsrv_cfg *cfg, const char *name, ...) {
    va_list va;
    bool ret;
    if (!cfg || b4r_is_empty(name))
        return false;
    va_start(va, name);
#define _IS_OPT(op) strcmp(name, op) == 0
#define _GET_OPT(mb, tp) do { \
    tp *val = va_arg(va, tp*); \
    ret = (val); \
    if (ret) \
        *val = cfg->mb; \
} while (0)
    if (_IS_OPT(BF_HTTPSRV_CFG_PORT))
        _GET_OPT(port, uint16_t);
    else if (_IS_OPT(BF_HTTPSRV_CFG_UPLDS_DIR))
        _GET_OPT(uplds_dir, char *);
    else if (_IS_OPT(BF_HTTPSRV_CFG_POST_BUF_SIZE))
        _GET_OPT(post_buffer_size, size_t);
    else if (_IS_OPT(BF_HTTPSRV_CFG_MAX_BODY_SIZE))
        _GET_OPT(max_body_size, unsigned long);
    else if (_IS_OPT(BF_HTTPSRV_CFG_MAX_PAYLD_SIZE))
        _GET_OPT(max_payld_size, unsigned long);
    else if (_IS_OPT(BF_HTTPSRV_CFG_MAX_UPLD_SIZE))
        _GET_OPT(max_upld_size, uint64_t);
    else if (_IS_OPT(BF_HTTPSRV_CFG_CONTENT_TYPE))
        _GET_OPT(content_type, char *);
    else if (_IS_OPT(BF_HTTPSRV_CFG_UUID_FUNC))
        _GET_OPT(uuid_func, bf_uuid_func);
#undef _IS_OPT
#undef _GET_OPT
    else
        ret = false;
    va_end(va);
    return ret;
}