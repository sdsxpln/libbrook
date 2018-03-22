#include <stdlib.h>
#include <brook.h>

/* NOTE: Error checking has been omitted for clarity. */

int main(void) {
    struct bk_httpsrv *srv;
    srv = bk_httpsrv_new();
    bk_httpsrv_setopt(srv, BK_HTTPSRV_OPT_PORT, 8080);
    bk_httpsrv_free(srv);
    return EXIT_SUCCESS;
}
