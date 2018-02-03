/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2018 Silvio Clecio, et al.
 *
 * This file is part of Brook4-REST.
 *
 * Brook4-REST is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brook4-REST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Brook4-REST.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef B4R_ASSERT_H
#define B4R_ASSERT_H

#ifndef __CYGWIN__
#ifndef _WIN32
#define _GNU_SOURCE
#include <errno.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#endif
#if defined(_WIN32) && !defined(__CYGWIN__)
#define __progname __argv[0]
#elif(_WIN32)
extern char __declspec(dllimport) *__progname;
#else
#define __progname program_invocation_short_name
#endif

#ifdef NDEBUG
#define ASSERT(expr) ((void) 0)
#else
#define ASSERT(expr)                                                        \
do {                                                                        \
    if (!(expr)) {                                                          \
        fprintf(stderr, "%s: %s:%d: %s: Assertion `%s' failed.\n",          \
            __progname, __FILE__, __LINE__, __FUNCTION__, #expr);           \
        fflush(stderr);                                                     \
        exit(1);                                                            \
    }                                                                       \
} while (0)
#endif

#endif /* B4R_ASSERT_H */
