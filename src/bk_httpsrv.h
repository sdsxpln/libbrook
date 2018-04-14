#ifndef BK_HTTPSRV_H
#define BK_HTTPSRV_H

#include "microhttpd.h"
#include "brook.h"

struct bk_httpsrv {
    struct MHD_Daemon *handle;
    bk_httpauth_cb auth_cb;
    bk_httpreq_cb req_cb;
    bk_httperr_cb err_cb;
    void *auth_cls;
    void *req_cls;
    void *err_cls;
    const char *uplds_dir;
    size_t post_bufsize;
    size_t max_payldsize;
    unsigned int thrd_pool_size;
    unsigned int con_timeout;
    unsigned int con_limit;
};

#endif /* BK_HTTPSRV_H */
