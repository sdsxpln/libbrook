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

#ifndef BK_MACROS_H
#define BK_MACROS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define _(String) (String) /* macro to make it easy to mark text for translation */

#define xstr(a) str(a) /* stringify the result of expansion of a macro argument */
#define str(a) #a

/* macro used for handling `Out of memory` errors. */
#ifdef NDEBUG
#define oom() exit(EXIT_FAILURE)
#else
#define oom()                                                                                   \
do {                                                                                            \
    if (isatty(fileno(stderr)) && (fprintf(stderr, _("%s:%d: %s: Out of memory\n"),             \
                                        __FILE__, __LINE__, __extension__ __FUNCTION__) > 0))   \
        fflush(stderr);                                                                         \
    exit(EXIT_FAILURE);                                                                         \
} while (0)
#endif

#endif /* BK_MACROS_H */
