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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bk_macros.h"
#include "bk_utils.h"
#include "brook.h"

#define BK_VERSION_STR xstr(BK_VERSION_MAJOR) "." xstr(BK_VERSION_MINOR) "." xstr(BK_VERSION_PATCH)

/* Version */

unsigned int bk_version(void) {
    return BK_VERSION_HEX;
}

const char *bk_version_str(void) {
    return BK_VERSION_STR;
}

/* Memory */

void *bk_alloc(size_t size) {
    void *p = malloc(size);
    if (p)
        memset(p, 0, size);
    return p;
}

void bk_free(void *ptr) {
    free(ptr);
}

/* String */

void bk__toasciilower(char *str) {
    while (*str) {
        if (/*isascii(*str) &&*/isupper(*str))
            *str = (char) tolower(*str);
        str++;
    }
}
