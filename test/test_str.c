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

#ifndef __ANDROID__
#include <string.h>
#endif
#include "brook.h"

static inline void test_str_strcpy(struct bk_str *str, const char *val, size_t len) {
    ASSERT(bk_str_strcpy(NULL, val, len) == -EINVAL);
    ASSERT(bk_str_strcpy(str, NULL, len) == -EINVAL);
    ASSERT(bk_str_strcpy(str, val, 0) == -EINVAL);

    bk_str_clear(str);
    ASSERT(bk_str_strcpy(str, val, len) == 0);
    ASSERT(bk_str_length(str) == len);
}

static inline void test_str_printf_va(struct bk_str *str, const char *fmt, va_list ap) {
    ASSERT(bk_str_printf_va(NULL, fmt, ap) == -EINVAL);
    ASSERT(bk_str_printf_va(str, NULL, ap) == -EINVAL);
#ifndef __ANDROID__
    ASSERT(bk_str_printf_va(str, fmt, NULL) == -EINVAL);
#endif

    bk_str_clear(str);
    bk_str_printf_va(str, fmt, ap);
    ASSERT(strcmp(bk_str_content(str), "abc123def456") == 0);

    /* the `test_str_printf_va()` is called and tested by `test_str_printf()`. */
}

static void test_str_printf(struct bk_str *str, const char *fmt, ...) {
    va_list ap;
    ASSERT(bk_str_printf(NULL, "") == -EINVAL);
    ASSERT(bk_str_printf(str, NULL) == -EINVAL);

    bk_str_clear(str);
    ASSERT(bk_str_printf(str, "") == 0);
    ASSERT(strlen(bk_str_content(str)) == 0);
    ASSERT(bk_str_printf(str, "%s%d", "abc", 123) == 0);
    ASSERT(strcmp(bk_str_content(str), "abc123") == 0);

    va_start(ap, fmt);
    test_str_printf_va(str, fmt, ap);
    va_end(ap);
}

static inline void test_str_content(struct bk_str *str, const char *val, size_t len) {
    ASSERT(!bk_str_content(NULL));

    bk_str_clear(str);
    ASSERT(strlen(bk_str_content(str)) == 0);
    bk_str_strcpy(str, val, len);
    ASSERT(strcmp(bk_str_content(str), val) == 0);
}

static inline void test_str_length(struct bk_str *str, const char *val, size_t len) {
    ASSERT(bk_str_length(NULL) == (size_t) -EINVAL);

    bk_str_clear(str);
    ASSERT(bk_str_length(str) == 0);
    bk_str_strcpy(str, val, len);
    ASSERT(bk_str_length(str) == len);
}

static inline void test_str_clear(struct bk_str *str, const char *val, size_t len) {
    ASSERT(bk_str_clear(NULL) == -EINVAL);

    bk_str_clear(str);
    bk_str_strcpy(str, val, len);
    ASSERT(bk_str_length(str) > 0);
    bk_str_clear(str);
    ASSERT(bk_str_length(str) == 0);
}

int main(void) {
    struct bk_str *str;
    const char *val = "abc123def456";
    const size_t len = strlen(val);

    str = bk_str_new();
    ASSERT(str);

    test_str_strcpy(str, val, len);
    test_str_printf(str, "%s%d%s%d", "abc", 123, "def", 456);
    /* test_str_printf_va()` is already called by `test_str_printf`. */
    test_str_content(str, val, len);
    test_str_length(str, val, len);
    test_str_clear(str, val, len);

    bk_str_free(str);
    return EXIT_SUCCESS;
}
