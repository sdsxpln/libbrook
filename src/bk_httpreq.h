#ifndef BK_HTTPREQ_H
#define BK_HTTPREQ_H

#include "microhttpd.h"
#include "bk_strmap.h"

struct bk__httpconvals_holder {
    struct bk_strmap **map;
    bool failed;
};

struct bk_httpreq {
    struct bk_strmap *headers;
    struct bk_strmap *cookies;
    struct bk_strmap *params;
    const char *version;
    const char *method;
    const char *path;
    void *userdata;
};

struct bk_httpreq *bk__httpreq_new(void);

void bk__httpreq_init(struct bk_httpreq *req, struct MHD_Connection *con, const char *version, const char *method,
                      const char *path);

void bk__httpreq_done(__BK_UNUSED void *cls, __BK_UNUSED struct MHD_Connection *con, void **con_cls,
                      __BK_UNUSED enum MHD_RequestTerminationCode toe);

#endif /* BK_HTTPREQ_H */
