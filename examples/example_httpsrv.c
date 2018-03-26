#include <stdio.h>
#include <stdlib.h>
#include <brook.h>

/* NOTE: Error checking has been omitted for clarity. */

static int req_cb(void *cls, struct bk_httpsrv_req *req, struct bk_httpsrv_res *res) {
    (void) cls;
    (void) req;
    (void) res;
    return 0;
}

int main(void) {
    struct bk_httpsrv *srv;
    srv = bk_httpsrv_new(req_cb, NULL);
    bk_httpsrv_start(srv, 8080, false);
    getchar();
    bk_httpsrv_stop(srv);
    bk_httpsrv_free(srv);
    return EXIT_SUCCESS;
}
