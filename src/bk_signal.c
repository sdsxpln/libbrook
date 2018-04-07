#include <stdlib.h>
#include <signal.h>
#ifndef _WIN32
#include <string.h>
#include <errno.h>
#include "bk_macros.h"
#endif
#include "brook.h"

int bk_signal(int sig, bk_signal_cb cb) {
#ifdef _WIN32
    if (signal(sig, cb) == SIG_ERR)
        return -errno;
#else
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    if (sigfillset(&sa.sa_mask))
        oom();
    sa.sa_handler = cb;
    if (sigaction(sig, &sa, NULL))
        return -errno;
#endif
    return 0;
}

int bk_unsignal(int sig) {
#ifdef _WIN32
    if (signal(sig, SIG_DFL) == SIG_ERR)
        return -errno;
#else
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    if (sigaction(sig, &sa, NULL))
        return -errno;
#endif
    return 0;
}

int bk_sigterm(bk_signal_cb cb) {
#ifdef SIGHUP
    if (bk_signal(SIGHUP, cb))
        return -errno;
#endif
    if (bk_signal(SIGINT, cb))
        return -errno;
#ifdef SIGQUIT
    if (bk_signal(SIGQUIT, cb))
        return -errno;
#endif
    if (bk_signal(SIGTERM, cb))
        return -errno;
#ifdef SIGBREAK
    if (bk_signal(SIGBREAK, cb))
        return -errno;
#endif
    return 0;
}
