#ifndef BK_HTTPUPLDS_H
#define BK_HTTPUPLDS_H

#include "utlist.h"
#include "microhttpd.h"
#include "bk_httpreq.h"
#include "bk_httpsrv.h"

struct bk_httpupld {
    struct bk_httpupld *next;
};

struct bk__httpuplds_holder {
    struct bk_httpsrv *srv;
    struct bk_httpreq *req;
};

bool bk__httpuplds_process(struct bk_httpsrv *srv, struct bk_httpreq *req, struct MHD_Connection *con,
                           const char *upld_data, size_t *upld_data_size, int *ret);

/*int bk__httpuplds_add();

void bk__httpuplds_cleanup();*/

#endif /*  */
