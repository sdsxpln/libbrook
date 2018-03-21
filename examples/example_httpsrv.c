#include <stdlib.h>
#include <brook.h>

int main(void) {
    struct bk_httpsrv *srv;
    srv = bk_httpsrv_new();
    bk_httpsrv_free(srv);
    return EXIT_SUCCESS;
}
