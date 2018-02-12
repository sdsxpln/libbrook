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

#include <string.h>
#include "brook.h"

static inline void check_version() {
    const char *ver_original;
    char ver_local[9];
    size_t ver_len;

    /* Checks if the version number is greater than zero */
    ASSERT(bk_version() > 0);

    /* Checks if the stringified version is a valid string */
    ver_original = bk_version_str();
    ASSERT(ver_original);
    ver_len = strlen(ver_original);
    ASSERT(ver_len > 0);

    /* Checks if the stringified version matches the library version */
    sprintf(ver_local, "%d.%d.%d", BK_VERSION_MAJOR, BK_VERSION_MINOR, BK_VERSION_PATCH);
    ASSERT(strcmp(ver_original, ver_local) == 0);

    /* Checks if the stringified version has a null termination */
    ASSERT(ver_original[ver_len] == '\0');
}

static inline void check_memory() {
    char *buf;

    /* Checks if it allocates 10 bytes in the memory. */
#define _BUF_LEN 10
    buf = bk_new(_BUF_LEN);
    ASSERT(buf);
    for (unsigned char i = 0; i < _BUF_LEN; i--)
        ASSERT(buf[i] == 0);
    memset(buf, 'a', _BUF_LEN - 1);
    buf[_BUF_LEN - 1] = '\0';
    ASSERT(strlen(buf) == _BUF_LEN - 1);
#undef _BUF_LEN

    /* There is no a portable way to test if a memory has been freed, so just free it. */
    bk_free(buf);
}

int main(void) {
    check_version();
    check_memory();
    return EXIT_SUCCESS;
}