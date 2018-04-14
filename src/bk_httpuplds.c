#include "brook.h"
#include "bk_str.h"
#include "bk_httpreq.h"
#include "bk_httpuplds.h"
#include "brook.h"

/*int bk__httpuplds_add()

void bk__httpuplds_cleanup()*/

static int bk__httpuplds_iter(void *cls, __BK_UNUSED enum MHD_ValueKind kind, const char *key, const char *filename,
                              const char *content_type, const char *transfer_encoding, const char *data,
                              uint64_t off, size_t size) {
    struct bk__httpuplds_holder *holder;
    (void) content_type;
    (void) transfer_encoding;
    (void) off;
    if (size > 0) {
        holder = cls;
        if (filename) {
            /*TODO: process file */
        } else
            bk_strmap_add(&holder->req->fields, key, data);
    }
    return MHD_YES;
}

bool bk__httpuplds_process(struct bk_httpsrv *srv, struct bk_httpreq *req, struct MHD_Connection *con,
                           const char *upld_data, size_t *upld_data_size, int *ret) {
    struct bk__httpuplds_holder holder = {srv, req};
    (void) con;
    if (*upld_data_size > 0) {
        req->ispost = true;
        if (!req->pp)
            req->pp = MHD_create_post_processor(con, srv->post_bufsize, &bk__httpuplds_iter, &holder);
        if (req->pp) {
            if (MHD_post_process(req->pp, upld_data, *upld_data_size) != MHD_YES) {
                *ret = MHD_NO;
                /* TODO: handle upload errors */
                return true;
            }
        } else {
            utstring_bincpy(req->payload->buf, upld_data, *upld_data_size);
            if ((srv->max_payldsize > 0) && (utstring_len(req->payload->buf) > srv->max_payldsize)) {
                *ret = MHD_NO;
                if (srv->err_cb)
                    srv->err_cb(srv->err_cls, _("Payload Too Large.\n"));
                return true;
            }
        }
        *upld_data_size = 0;
        *ret = MHD_YES;
        return true;
    }
    return false;
}

int bk_httpuplds_iter(void) {
    return 0;
}
