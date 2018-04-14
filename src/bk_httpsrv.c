#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "microhttpd.h"
#include "bk_macros.h"
#include "brook.h"
#include "bk_httpauth.h"
#include "bk_httpuplds.h"
#include "bk_httpreq.h"
#include "bk_httpres.h"
#include "bk_httpsrv.h"

static void bk__httperr_cb(__BK_UNUSED void *cls, const char *err) {
    if (isatty(fileno(stderr)) && (fprintf(stderr, "%s", err) > 0))
        fflush(stderr);
}

static void bk__httpsrv_oel(void *cls, const char *fmt, va_list ap) {
    struct bk_httpsrv *srv = cls;
    va_list ap_cpy;
    size_t size;
    char *err;
    va_copy(ap_cpy, ap);
    size = vsnprintf(NULL, 0, fmt, ap_cpy) + sizeof(char);
    va_end(ap_cpy);
    if (!(err = bk_alloc(size)))
        oom();
    vsnprintf(err, size, fmt, ap);
    srv->err_cb(srv->err_cls, err);
    bk_free(err);
}

static int bk__httpsrv_ahc(void *cls, struct MHD_Connection *con, const char *url, const char *method,
                           const char *version, const char *upld_data, size_t *upld_data_size, void **con_cls) {
    struct bk_httpsrv *srv = cls;
    struct bk_httpauth auth;
    struct bk_httpres res;
    if (!*con_cls) {
        if (srv->auth_cb) {
            bk__httpauth_init(&auth, con);
            res.ret = srv->auth_cb(srv->auth_cls, &auth);
            if (!bk__httpauth_done(&auth, &res.ret))
                return res.ret;
        }
        *con_cls = bk__httpreq_new(); /* `*con_cls` keeps the request instance */
        return MHD_YES;
    }
    if (bk__httpuplds_process(srv, *con_cls, con, upld_data, upld_data_size, &res.ret))
        return res.ret;
    bk__httpreq_prepare(*con_cls, con, version, method, url);
    bk__httpres_init(&res, con);
    srv->req_cb(srv->req_cls, *con_cls, &res);
    return bk__httpres_done(&res);
}

struct bk_httpsrv *bk_httpsrv_new2(bk_httpauth_cb auth_cb, void *auth_cls, bk_httpreq_cb req_cb, void *req_cls,
                                   bk_httperr_cb err_cb, void *err_cls) {
    struct bk_httpsrv *srv = bk_alloc(sizeof(struct bk_httpsrv));
    if (!req_cb || !err_cb)
        return NULL;
    srv->auth_cb = auth_cb;
    srv->auth_cls = auth_cls;
    srv->req_cb = req_cb;
    srv->req_cls = req_cls;
    srv->err_cb = err_cb;
    srv->err_cls = err_cls;
#ifdef __ANDROID__
    srv->post_bufsize = 1024; /* 1 Kb */
    srv->max_payldsize = 524288; /* 512 kB */
#else
    srv->post_bufsize = 4096; /* 4 kB */
    srv->max_payldsize = 4194304; /* 4 MB */
#endif
    return srv;
}

struct bk_httpsrv *bk_httpsrv_new(bk_httpreq_cb cb, void *cls) {
    return bk_httpsrv_new2(NULL, NULL, cb, cls, bk__httperr_cb, NULL);
}

void bk_httpsrv_free(struct bk_httpsrv *srv) {
    if (!srv)
        return;
    bk_httpsrv_stop(srv);
    bk_free(srv);
}

int bk_httpsrv_setopt_va(struct bk_httpsrv *srv, enum BK_HTTPSRV_OPT opt, va_list ap) {
    if (!srv)
        return -EINVAL;
    switch (opt) {
        case BK_HTTPSRV_OPT_UNKNOWN:
            break;
        case BK_HTTPSRV_OPT_UPLD_DIR:
            srv->uplds_dir = va_arg(ap, const char *);
            break;
        case BK_HTTPSRV_OPT_POST_BUFSIZE:
            srv->post_bufsize = va_arg(ap, size_t);
            break;
        case BK_HTTPSRV_OPT_MAX_PAYLDSIZE:
            srv->max_payldsize = va_arg(ap, size_t);
            break;
        case BK_HTTPSRV_OPT_THRD_POOL_SIZE:
            srv->thrd_pool_size = va_arg(ap, unsigned int);
            break;
        case BK_HTTPSRV_OPT_CON_TIMEOUT:
            srv->con_timeout = va_arg(ap, unsigned int);
            break;
        case BK_HTTPSRV_OPT_CON_LIMIT:
            srv->con_limit = va_arg(ap, unsigned int);
            break;
        default:;
    }
    return 0;
}

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
int bk_httpsrv_setopt(struct bk_httpsrv *srv, enum BK_HTTPSRV_OPT opt, ...) {
    va_list ap;
    int ret;
    va_start(ap, opt);
    ret = bk_httpsrv_setopt_va(srv, opt, ap);
    va_end(ap);
    return ret;
}

int bk_httpsrv_start(struct bk_httpsrv *srv, uint16_t port, bool threaded) {
    if (!srv || port <= 0)
        return -EINVAL;
    if (!(srv->handle = MHD_start_daemon(
            MHD_USE_DUAL_STACK | MHD_USE_ERROR_LOG |
            (threaded ? MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_THREAD_PER_CONNECTION : MHD_USE_AUTO_INTERNAL_THREAD),
            port, NULL, NULL,
            bk__httpsrv_ahc, srv,
            MHD_OPTION_EXTERNAL_LOGGER, bk__httpsrv_oel, srv,
            MHD_OPTION_NOTIFY_COMPLETED, bk__httpreq_done, NULL,
            MHD_OPTION_END)))
        return -ECONNABORTED;
    return 0;
}

int bk_httpsrv_stop(struct bk_httpsrv *srv) {
    if (!srv)
        return -EINVAL;
    if (srv->handle) {
        MHD_stop_daemon(srv->handle);
        srv->handle = NULL;
    }
    return 0;
}
