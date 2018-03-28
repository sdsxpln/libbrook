#ifndef BK_HTTPSRV_H
#define BK_HTTPSRV_H

#include "microhttpd.h"
#include "bk_strmap.h"

struct bk_httpsrv_req {
    struct bk_strmap *headers;
};

struct bk_httpsrv_res {
    struct bk_strmap *headers;
};

struct bk_httpsrv {
    struct MHD_Daemon *daemon;
    bk_httpsrv_req_cb req_cb;
    void *req_cls;
    bk_httpsrv_err_cb err_cb;
    void *err_cls;
    char *upload_dir;
};

#endif /* BK_HTTPSRV_H */
