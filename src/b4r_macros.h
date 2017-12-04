/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2017 Silvio Clecio, et al.
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

#ifndef B4R_MACROS_H
#define B4R_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * Macro to make it easy to mark text for translation. Note that
 * we do not actually call gettext() in libbrook, but we do make
 * it easy to create a ".po" file so that applications that do
 * want to translate error messages can do so.
 */

#if !defined(UNIX) && (defined(unix) || defined(__unix__) || defined(__unix))
# define UNIX 1
#endif
#if !defined(WIN) && (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
# define WIN 1
#endif

#ifndef _
# define _(String) (String)
#endif

#define _B4R_STR(s) #s

#ifdef _LP64
# define __PRIS_PREFIX "z"
#else
# define __PRIS_PREFIX
#endif

#define PRIdS __PRIS_PREFIX "d"
#define PRIxS __PRIS_PREFIX "x"
#define PRIuS __PRIS_PREFIX "u"
#define PRIXS __PRIS_PREFIX "X"
#define PRIoS __PRIS_PREFIX "o"

#ifndef DIR_SEP
# ifdef WIN
#  define DIR_SEP "\\"
# else
#  define DIR_SEP "/"
# endif
#endif

/* TODO: on Linux, use syslog. */
/* TODO: on Android, use its log system. */
/* TODO: on Windows, use its Event Logs. */
#define _B4R_LOG(enabled, ...) do { \
    if (enabled) \
        fprintf(stderr, __VA_ARGS__); \
} while (0)

#define _B4R_DIE(...) do { \
    _B4R_LOG(true, __VA_ARGS__); \
    abort(); \
} while (0)

#define _B4R_NEW(p) do { \
    *(p) = calloc(1, sizeof(**(p))); \
    if (!*(p)) { \
        errno = ENOMEM; \
        _B4R_DIE("%s", strerror(errno)); \
    } \
} while (0)

#define _B4R_FREE(p) free(p)

#endif