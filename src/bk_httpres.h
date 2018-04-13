#ifndef BK_HTTPRES_H
#define BK_HTTPRES_H

#include "microhttpd.h"
#include "bk_strmap.h"

struct bk_httpres {
    struct MHD_Connection *con;
    struct MHD_Response *handle;
    struct bk_strmap *headers;
    unsigned int status;
    int ret;
};

void bk__httpres_init(struct bk_httpres *res, struct MHD_Connection *con);

int bk__httpres_done(struct bk_httpres *res);

#endif /* BK_HTTPRES_H */
