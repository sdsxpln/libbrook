#ifndef BK_HTTPAUTH_H
#define BK_HTTPAUTH_H

#include <stdbool.h>
#include "microhttpd.h"

struct bk_httpauth {
    struct MHD_Connection *con;
    struct MHD_Response *handle;
    char *realm;
    char *usr;
    char *pwd;
    char *justification;
    char *content_type;
    bool canceled;
};

void bk__httpauth_init(struct bk_httpauth *auth, struct MHD_Connection *con);

bool bk__httpauth_done(struct bk_httpauth *auth, int *ret);

#endif /* BK_HTTPAUTH_H */
