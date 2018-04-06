#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "bk_macros.h"
#include "brook.h"

int bk_signal(int sig, bk_signal_cb cb) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    if (sigfillset(&sa.sa_mask))
        oom();
    sa.sa_handler = cb;
    if (sigaction(sig, &sa, NULL))
        return -errno;
    return 0;
}

void bk_unsignal(int sig) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    if (sigaction(sig, &sa, NULL))
        oom();
}

int bk_sigterm(bk_signal_cb cb) {
    if (bk_signal(SIGHUP, cb))
        return -errno;
    if (bk_signal(SIGINT, cb))
        return -errno;
    if (bk_signal(SIGQUIT, cb))
        return -errno;
    if (bk_signal(SIGTERM, cb))
        return -errno;
    return 0;
}
