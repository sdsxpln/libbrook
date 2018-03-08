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

#ifndef BK_ASSERT_H
#define BK_ASSERT_H

#ifndef _WIN32
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <errno.h>
#endif
#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32)
#define __progname __argv[0]
#elif(_WIN32)
extern char __declspec(dllimport) *__progname;
#else
#ifdef __USE_GNU
#define __progname program_invocation_short_name
#else
extern const char *__progname;
#endif
#endif

#ifdef NDEBUG
#define ASSERT(expr) ((void) 0)
#else
#define ASSERT(expr)                                                            \
do {                                                                            \
    if (!(expr)) {                                                              \
        fprintf(stderr, "%s: %s:%d: %s: Assertion `%s' failed.\n",              \
            __progname, __FILE__, __LINE__, __extension__ __FUNCTION__, #expr); \
        fflush(stderr);                                                         \
        exit(EXIT_FAILURE);                                                     \
    }                                                                           \
} while (0)
#endif

#endif /* BK_ASSERT_H */
