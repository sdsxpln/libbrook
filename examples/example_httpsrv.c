#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <brook.h>

/* NOTE: Error checking has been omitted for clarity. */

static void req_cb(__BK_UNUSED void *cls, __BK_UNUSED struct bk_httpreq *req, struct bk_httpres *res) {
    struct bk_str *body = bk_httpres_body(res);
    bk_str_printf(body, "<html><head><title>Hello world</title></head><body>Hello world</body></html>");
}

int main(void) {
    struct bk_httpsrv *srv = bk_httpsrv_new(req_cb, NULL);
    if (bk_httpsrv_start(srv, 8080, false) == 0)
        getchar();
    bk_httpsrv_free(srv);
    return errno;
}
