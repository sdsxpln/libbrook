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

#ifndef _ERRNO_H
#include <errno.h>
#endif

#include <string.h>
#include "brook.h"
#include "bk_utils.h"

static inline void test_version(void) {
    const char *ver_original;
    char ver_local[9];
    size_t ver_len;

    ASSERT(bk_version() > 0);

    ver_original = bk_version_str();
    ASSERT(ver_original);
    ver_len = strlen(ver_original);
    ASSERT(ver_len > 0);

#ifdef _MSC_VER
    sprintf_s(ver_local, sizeof(ver_local)
#else
    sprintf(ver_local
#endif
            , "%d.%d.%d", BK_VERSION_MAJOR, BK_VERSION_MINOR, BK_VERSION_PATCH);
    ASSERT(strcmp(ver_original, ver_local) == 0);

    ASSERT(ver_original[ver_len] == '\0');
}

static inline void test_memory(void) {
    char *buf;
#define _BUF_LEN 10
    buf = bk_alloc(_BUF_LEN);
    ASSERT(buf);
    for (unsigned char i = 0; i < _BUF_LEN; i++)
        ASSERT(buf[i] == 0);
    memset(buf, 'a', _BUF_LEN - 1);
    buf[_BUF_LEN - 1] = '\0';
    ASSERT(strlen(buf) == _BUF_LEN - 1);
#undef _BUF_LEN
    bk_free(buf);
}

static inline void test__toasciilower(void) {
    char str[100];
    size_t len;
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "ABC");
    len = strlen(str);
    ASSERT(bk__toasciilower(NULL, len) == -EINVAL);
    ASSERT(bk__toasciilower(str, 0) == -EINVAL);
    memset(str, 0, sizeof(str));
    ASSERT(bk__toasciilower(str, 0) == -EINVAL);

    memset(str, 0, sizeof(str));
    len = 10;
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(*str == 0);
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "A");
    len = strlen(str);
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(strcmp(str, "a") == 0);
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "ABC");
    len = strlen(str);
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(strcmp(str, "abc") == 0);
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "ABC123 def456");
    len = strlen(str);
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(strcmp(str, "abc123 def456") == 0);
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "ABC");
    len = strlen(str) * 2;
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(strcmp(str, "abc") == 0);
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "ABÇñãÁÊD");
    len = strlen(str);
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(strcmp(str, "abÇñãÁÊd") == 0);
#ifdef _MSC_VER
    sprintf_s(str, sizeof(str)
#else
    sprintf(str
#endif
            , "AB");
    len = strlen(str);
#ifdef _MSC_VER
    strcat_s(str, sizeof(str)
#else
    strcat(str
#endif
            , "CD");
    ASSERT(bk__toasciilower(str, len) == 0);
    ASSERT(strcmp(str, "abCD") == 0);
}

int main(void) {
    test_version();
    test_memory();
    test__toasciilower();
    return EXIT_SUCCESS;
}