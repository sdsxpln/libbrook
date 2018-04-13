#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "microhttpd.h"
#include "bk_macros.h"
#include "bk_utils.h"
#include "bk_httpauth.h"

void bk__httpauth_init(struct bk_httpauth *auth, struct MHD_Connection *con) {
    memset(auth, 0, sizeof(struct bk_httpauth));
    auth->con = con;
    auth->usr = MHD_basic_auth_get_username_password(con, &auth->pwd);
}

bool bk__httpauth_done(struct bk_httpauth *auth, int *ret) {
    bk_free(auth->usr);
    bk_free(auth->pwd);
    if (*ret) {
        *ret = MHD_YES;
        goto done;
    }
    if (auth->canceled) {
        *ret = MHD_NO;
        goto done;
    }
    auth->handle = MHD_create_response_from_buffer(strlen(auth->justification), auth->justification,
                                                   MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(auth->handle, MHD_HTTP_HEADER_CONTENT_TYPE, auth->content_type);
    bk_free(auth->content_type);
    *ret = MHD_queue_basic_auth_fail_response(auth->con, auth->realm ? auth->realm : _("Brook realm"), auth->handle);
    bk_free(auth->realm);
    MHD_destroy_response(auth->handle);
    return false;
done:
    bk_free(auth->realm);
    bk_free(auth->justification);
    bk_free(auth->content_type);
    return *ret == MHD_YES;
}

int bk_httpauth_setrealm(struct bk_httpauth *auth, const char *realm) {
    if (!auth || !realm)
        return -EINVAL;
    if (auth->realm)
        return -EALREADY;
    if (!(auth->realm = strdup(realm)))
        oom();
    return 0;
}

int bk_httpauth_deny(struct bk_httpauth *auth, const char *justification, const char *content_type) {
    if (!auth || !justification || !content_type)
        return -EINVAL;
    if (auth->justification)
        return -EALREADY;
    if (!(auth->justification = strdup(justification)) || !(auth->content_type = strdup(content_type))) {
        bk_free(auth->justification);
        bk_free(auth->content_type);
        oom();
    }
    return 0;
}

int bk_httpauth_cancel(struct bk_httpauth *auth) {
    if (!auth)
        return -EINVAL;
    auth->canceled = true;
    return 0;
}

const char *bk_httpauth_usr(struct bk_httpauth *auth) {
    return auth ? auth->usr : NULL;
}

const char *bk_httpauth_pwd(struct bk_httpauth *auth) {
    return auth ? auth->pwd : NULL;
}
