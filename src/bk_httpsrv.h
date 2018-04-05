#ifndef BK_HTTPSRV_H
#define BK_HTTPSRV_H

#include "microhttpd.h"
#include "bk_strmap.h"
#include "brook.h"

struct bk_httpreq {
    struct bk_strmap *headers;
};

struct bk_httpres {
    struct MHD_Response *handle;
    struct bk_strmap *headers;
    unsigned int status;
};

struct bk_httpsrv {
    struct MHD_Daemon *handle;
    bk_httpreq_cb req_cb;
    void *req_cls;
    bk_httperr_cb err_cb;
    void *err_cls;
    char *upload_dir;
};

#endif /* BK_HTTPSRV_H */
