#ifndef BF_HTTPSRV_REQ_UPLDS_H
#define BF_HTTPSRV_REQ_UPLDS_H

#include <stdio.h>
#include <microhttpd.h>
#include "bf_httpsrv_cfg.h"

struct bf_httpsrv_req_upld {
    struct bf_httpsrv_req_upld *next;
    void *stream;
    uint64_t size;
    char *dir;
    char *name;
    char *orig_name;
    char *dest_name;
    char *field;
    char *mime;
    char *encoding;
    bool failed;
    char err[BF_ERR_SIZE];
};

bool _bf_httpsrv_req_uplds_new(struct bf_httpsrv_req_upld **uplds, struct bf_httpsrv_req_upld **upld,
                               bf_uuid_func uuid_func, const char *uplds_dir, const char *name, const char *field,
                               const char *mime, const char *encoding);

void _bf_httpsrv_req_uplds_free(struct bf_httpsrv_req_upld *upld);

void _bf_httpsrv_req_upds_cleanup(struct bf_httpsrv_req_upld *uplds);

void *_bf_httpsrv_req_upld_file_prepare_cb(void *cls, struct bf_httpsrv_req_upld *upld);

bool _bf_httpsrv_req_upld_file_save_cb(void *cls, struct bf_httpsrv_req_upld *upld, bool overwritten);

bool _bf_httpsrv_req_upld_file_write_cb(void *cls, struct bf_httpsrv_req_upld *upld,
                                        const char *buf, size_t size);

bool _bf_httpsrv_req_uplds_process(struct b4r_httpsrv_req *req, struct MHD_Connection *con, void **con_cls,
                                   const char *upld_data, size_t *upld_data_size, int *ret);

#endif