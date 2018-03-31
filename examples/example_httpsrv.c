#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <brook.h>

/* NOTE: Error checking has been omitted for clarity. */

static int req_cb(void *cls, struct bk_httpreq *req, struct bk_httpres *res) {
    (void) cls;
    (void) req;
    bk_str_printf(bk_httpres_body(res), "%s",
                  "<html><head><title>Hello world</title></head><body>Hello world</body></html>");
    return 0;
}

int main(void) {
    struct bk_httpsrv *srv = bk_httpsrv_new(req_cb, NULL);
    if (bk_httpsrv_start(srv, 8080, false) == 0)
        getchar();
    bk_httpsrv_free(srv);
    return errno;
}
