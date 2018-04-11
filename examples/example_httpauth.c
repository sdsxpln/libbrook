#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <brook.h>

/* NOTE: Error checking has been omitted for clarity. */

#define PORT 8080

static bool match_text(const char *s1, const char *s2) {
    if (!s1 || !s2)
        return false;
    return strcmp(s1, s2) == 0;
}

static bool auth_cb(__BK_UNUSED void *cls, struct bk_httpauth *auth) {
    bool pass;
    bk_httpauth_setrealm(auth, "My realm");
    if (!(pass = match_text(bk_httpauth_usr(auth), "abc") && match_text(bk_httpauth_pwd(auth), "123")))
        bk_httpauth_deny(auth,
                         "<html><head><title>Denied</title></head><body><font color=\"red\">Go away</font></body></html>",
                         "text/html; charset=utf-8");
    return pass;
}

static void err_cb(__BK_UNUSED void *cls, const char *err) {
    fprintf(stderr, "%s", err);
    fflush(stderr);
}

static void req_cb(__BK_UNUSED void *cls, __BK_UNUSED struct bk_httpreq *req, struct bk_httpres *res) {
    bk_httpres_send(res,
                    "<html><head><title>Secret</title></head><body><font color=\"green\">Secret page</font></body></html>",
                    "text/html; charset=utf-8", 200);
}

int main(void) {
    struct bk_httpsrv *srv = bk_httpsrv_new2(auth_cb, NULL, req_cb, NULL, err_cb, NULL);
    if (bk_httpsrv_start(srv, PORT, false) == 0) {
        printf("Server running at http://localhost:%d\n", PORT);
        getchar();
    }
    bk_httpsrv_free(srv);
    return errno;
}
