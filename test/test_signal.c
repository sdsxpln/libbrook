/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\
 *
 *   –– a small library which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2018 Silvio Clecio <silvioprog@gmail.com>
 *
 * This file is part of Brook library.
 *
 * Brook library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brook library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Brook library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bk_assert.h"

#include "brook.h"
#include <signal.h>

#ifdef _WIN32
#define SIGUSR1 1
#define SIGUSR2 2
#endif

bool signaled1 = false;
bool signaled2 = false;
unsigned char signaled_times1 = 0;
unsigned char signaled_times2 = 0;

static void signal_cb1(int sig) {
    signaled1 = sig == SIGUSR1 || sig == SIGUSR2;
}

static void signal_cb2(int sig) {
    signaled2 = sig == SIGUSR1;
}

static void sigterm_cb(int sig) {
#ifdef SIGHUP
    if (sig == SIGHUP)
        signaled_times2++;
#endif
    if (sig == SIGINT)
        signaled_times2++;
    /* raise() cannot raise SIGQUIT. */
    if (sig == SIGTERM)
        signaled_times2++;
#ifdef SIGBREAK
    if (sig == SIGBREAK)
        signaled_times2++;
#endif
}

static void test_signal(void) {
    ASSERT(bk_signal(0, signal_cb1) == -EINVAL);
    ASSERT(bk_signal(SIGUSR1, NULL) == 0);
    ASSERT(bk_signal(SIGUSR1, SIG_DFL) == 0);
    ASSERT(bk_signal(SIGUSR1, signal_cb1) == 0);
    ASSERT(!signaled1);
    raise(SIGUSR1);
    ASSERT(signaled1);
    signaled1 = false;
    ASSERT(!signaled1);
    ASSERT(bk_signal(SIGUSR2, signal_cb1) == 0);
    raise(SIGUSR2);
    ASSERT(signaled1);
}

static void test_unsignal(void) {
    ASSERT(bk_unsignal(0) == -EINVAL);
    ASSERT(bk_unsignal(SIGUSR1) == 0);
    signaled1 = false;
    signaled2 = false;
    ASSERT(!signaled1);
    ASSERT(!signaled2);
    ASSERT(signal(SIGUSR1, signal_cb2) != SIG_ERR);
    raise(SIGUSR1);
    ASSERT(!signaled1);
    ASSERT(signaled2);
}

static void test_sigterm(void) {
    bk_sigterm(sigterm_cb);
#ifdef SIGHUP
    signaled_times1++;
    raise(SIGHUP);
#endif
#ifdef SIGINT
    signaled_times1++;
    raise(SIGINT);
#endif
    /* raise() cannot raise SIGQUIT. */
#ifdef SIGTERM
    signaled_times1++;
    raise(SIGTERM);
#endif
#ifdef SIGBREAK
    signaled_times1++;
    raise(SIGBREAK);
#endif
    ASSERT(signaled_times1 == signaled_times2);
}

int main(void) {
    test_signal();
    test_unsignal();
    test_sigterm();
    return EXIT_SUCCESS;
}
