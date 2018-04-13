#include <stdlib.h>
#include <string.h>
#include "microhttpd.h"
#include "bk_macros.h"
#include "brook.h"
#include "bk_httpreq.h"

static void bk__httpreq_cleanup(struct bk_httpreq *req) {
    bk_strmap_cleanup(&req->headers);
    bk_strmap_cleanup(&req->cookies);
    bk_strmap_cleanup(&req->params);
}

static int bk__httpreq_iter(void *cls, __BK_UNUSED enum MHD_ValueKind kind, const char *key, const char *val) {
    struct bk__httpconvals_holder *holder = cls;
    return (holder->failed = (bk_strmap_add(holder->map, key, val) != 0)) ? MHD_NO : MHD_YES;
}

struct bk_httpreq *bk__httpreq_new(void) {
    return bk_alloc(sizeof(struct bk_httpreq));
}

void bk__httpreq_init(struct bk_httpreq *req, struct MHD_Connection *con, const char *version, const char *method,
                      const char *path) {
    struct bk__httpconvals_holder holder;
    memset(&holder, 0, sizeof(struct bk__httpconvals_holder));
    holder.map = &req->headers;
    MHD_get_connection_values(con, MHD_HEADER_KIND, bk__httpreq_iter, &holder);
    if (holder.failed)
        goto fail_oom;
    holder.map = &req->cookies;
    MHD_get_connection_values(con, MHD_COOKIE_KIND, bk__httpreq_iter, &holder);
    if (holder.failed)
        goto fail_oom;
    holder.map = &req->params;
    MHD_get_connection_values(con, MHD_GET_ARGUMENT_KIND, bk__httpreq_iter, &holder);
    if (holder.failed)
        goto fail_oom;
    req->version = version;
    req->method = method;
    req->path = path;
    return;
fail_oom:
    bk__httpreq_cleanup(req);
    oom();
}

void bk__httpreq_done(__BK_UNUSED void *cls, __BK_UNUSED struct MHD_Connection *con, void **con_cls,
                      __BK_UNUSED enum MHD_RequestTerminationCode toe) {
    struct bk_httpreq *req = *con_cls;
    if (req) {
        bk__httpreq_cleanup(req);
        bk_free(req);
    }
    *con_cls = NULL;
}

struct bk_strmap **bk_httpreq_headers(struct bk_httpreq *req) {
    return req ? &req->headers : NULL;
}

struct bk_strmap **bk_httpreq_cookies(struct bk_httpreq *req) {
    return req ? &req->cookies : NULL;
}

struct bk_strmap **bk_httpreq_params(struct bk_httpreq *req) {
    return req ? &req->params : NULL;
}

const char *bk_httpreq_version(struct bk_httpreq *req) {
    return req ? req->version : NULL;
}

const char *bk_httpreq_method(struct bk_httpreq *req) {
    return req ? req->method : NULL;
}

const char *bk_httpreq_path(struct bk_httpreq *req) {
    return req ? req->path : NULL;
}

int bk_httpreq_setuserdata(struct bk_httpreq *req, void *data) {
    if (!req || !data)
        return -EINVAL;
    req->userdata = data;
    return 0;
}

void *bk_httpreq_userdata(struct bk_httpreq *req) {
    return req ? req->userdata : NULL;
}
