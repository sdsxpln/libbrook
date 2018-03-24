#ifndef BK_HTTPSRV_H
#define BK_HTTPSRV_H

#include "microhttpd.h"

struct bk_httpsrv {
    struct MHD_Daemon *daemon;
    char *upload_dir;
};

#endif /* BK_HTTPSRV_H */