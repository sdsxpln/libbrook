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

#include <errno.h>

#define _(String) (String) /* macro to make it easy to mark text for translation */

#define xstr(a) str(a) /* stringify the result of expansion of a macro argument */
#define str(a) #a

/* oom() - macro used by libs `ut` for handling `Out of memory` errors. */
#define oom()                               \
do {                                        \
    fprintf(stderr, _("Out of memory\n"));  \
    fflush(stderr);                         \
    exit(ENOMEM);                           \
} while (0)

#endif /* BK_MACROS_H */
