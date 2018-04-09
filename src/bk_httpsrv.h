#ifndef BK_HTTPSRV_H
#define BK_HTTPSRV_H

#include <stdbool.h>
#include "microhttpd.h"
#include "bk_strmap.h"
#include "brook.h"

struct bk_httpauth {
    char *realm;
    char *usr;
    char *pwd;
    bool canceled;
};

struct bk_httpreq {
    struct bk_strmap *headers;
};

struct bk_httpres {
    struct MHD_Connection *con;
    struct MHD_Response *handle;
    struct bk_strmap *headers;
    unsigned int status;
    int ret;
};

struct bk_httpsrv {
    struct MHD_Daemon *handle;
    bk_httpauth_cb auth_cb;
    bk_httpreq_cb req_cb;
    bk_httperr_cb err_cb;
    void *auth_cls;
    void *req_cls;
    void *err_cls;
    char *upload_dir;
};

#endif /* BK_HTTPSRV_H */
