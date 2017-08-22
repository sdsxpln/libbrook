#ifndef BF_HTTPSRV_REQ_H
#define BF_HTTPSRV_REQ_H

#include <stdbool.h>
#include "utstring.h"
#include <microhttpd.h>
#include "b4r_hs.h"
#include <brook.h>

struct b4r_httpsrv_req {
    struct bf_httpsrv *owner;
    struct bf_httpsrv_res *res;
    struct MHD_Connection *con;
    struct MHD_PostProcessor *post_proc;
    struct b4r_hs *headers;
    struct b4r_hs *cookies;
    struct b4r_hs *params;
    struct b4r_hs *fields;
    struct bf_httpsrv_req_upld *uplds;
    struct bf_httpsrv_req_upld *cur_upld;
    bool receiving;
    bool uploading;
    bool aborted;
    bool done;
    size_t total_upld_size;
    size_t content_len;
    char *id;
    char *uri;
    const char *content_type;
    const char *version;
    const char *method;
    const char *path;
    UT_string *payload;
};

void *_bf_httpsrv_req_init_cb(void *cls, const char *uri, struct MHD_Connection *con);

void _bf_httpsrv_req_fini_cb(void *cls, struct MHD_Connection *con, void **con_cls,
                             enum MHD_RequestTerminationCode toe);

int _bf_httpsrv_req_cb(void *cls, struct MHD_Connection *con, const char *url, const char *method, const char *version,
                       const char *upld_data, size_t *upld_data_size, void **con_cls);

void _bf_httpsrv_req_err(struct b4r_httpsrv_req *req, const char *fmt, ...) __format(2, 3);

#endif