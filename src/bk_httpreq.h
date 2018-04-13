#ifndef BK_HTTPREQ_H
#define BK_HTTPREQ_H

#include <stdbool.h>
#include "microhttpd.h"
#include "brook.h"
#include "bk_httpuplds.h"

struct bk__httpconvals_holder {
    struct bk_strmap **map;
    bool failed;
};

struct bk_httpreq {
    struct MHD_PostProcessor *pp;
    struct bk_httpupld *uplds;
    struct bk_strmap *headers;
    struct bk_strmap *cookies;
    struct bk_strmap *params;
    struct bk_str *payload;
    const char *version;
    const char *method;
    const char *path;
    void *userdata;
    bool ispost;
};

struct bk_httpreq *bk__httpreq_new(void);

void bk__httpreq_prepare(struct bk_httpreq *req, struct MHD_Connection *con, const char *version, const char *method,
                         const char *path);

void bk__httpreq_done(__BK_UNUSED void *cls, __BK_UNUSED struct MHD_Connection *con, void **con_cls,
                      __BK_UNUSED enum MHD_RequestTerminationCode toe);

#endif /* BK_HTTPREQ_H */
