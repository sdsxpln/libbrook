#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <brook.h>

/* NOTE: Error checking has been omitted for clarity. */

#define PORT 8080

static void req_cb(__BK_UNUSED void *cls, __BK_UNUSED struct bk_httpreq *req, struct bk_httpres *res) {
    bk_httpres_send(res, "<html><head><title>Hello world</title></head><body>Hello world</body></html>",
                    "text/html; charset=utf-8", 200);
}

int main(void) {
    struct bk_httpsrv *srv = bk_httpsrv_new(req_cb, NULL);
    if (bk_httpsrv_start(srv, PORT, false) == 0) {
        printf("Server running at http://localhost:%d\n", PORT);
        getchar();
    }
    bk_httpsrv_free(srv);
    return errno;
}
