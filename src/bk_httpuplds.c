#include "bk_str.h"
#include "bk_httpreq.h"
#include "bk_httpuplds.h"
#include "brook.h"

/*int bk__httpuplds_add()

void bk__httpuplds_cleanup()*/

bool bk__httpuplds_process(struct bk_httpsrv *srv, struct bk_httpreq *req, struct MHD_Connection *con,
                           const char *upld_data, size_t *upld_data_size, int *ret) {
    (void) srv;
    (void) con;
    if (*upld_data_size > 0) {
        req->payload = bk_str_new();
        utstring_bincpy(req->payload->buf, upld_data, *upld_data_size);
        *upld_data_size = 0;
        *ret = MHD_YES;
        return true;
    }
    return false;
}

int bk_httpuplds_iter(void) {
    return 0;
}
