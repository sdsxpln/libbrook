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

static inline void test_str_write(struct bk_str *str, const char *val, size_t len) {
    size_t res_len;

    ASSERT(bk_str_write(NULL, val, len) == -EINVAL);
    ASSERT(bk_str_write(str, NULL, len) == -EINVAL);
    ASSERT(bk_str_write(str, val, 0) == -EINVAL);

    bk_str_clear(str);
    ASSERT(bk_str_write(str, val, len) == 0);
    res_len = 0;
    bk_str_length(str, &res_len);
    ASSERT(res_len == len);
}

static inline void test_str_read(struct bk_str *str, const char *val, size_t len) {
    char res[16 * sizeof(char)];
    size_t res_len;

    ASSERT(bk_str_read(NULL, res, &res_len) == -EINVAL);
    ASSERT(bk_str_read(str, NULL, &res_len) == -EINVAL);
    ASSERT(bk_str_read(str, res, NULL) == -EINVAL);
    ASSERT(bk_str_read(str, res, (size_t *) 0) == -EINVAL);

    bk_str_clear(str);
    res_len = 10;
	memcpy(res, val, len);
	res[len] = '\0';
    ASSERT(strlen(res) == len);
    ASSERT(bk_str_read(str, res, &res_len) == 0);
    ASSERT(res_len == 0);
    ASSERT(strlen(res) == 0);

    bk_str_write(str, val, len);
    res_len = sizeof(char);
    ASSERT(bk_str_read(str, res, &res_len) == 0);
    ASSERT(res_len == sizeof(char));
    ASSERT(memcmp(val, res, sizeof(char)) == 0);

    res_len = len + len;
    ASSERT(bk_str_read(str, res, &res_len) == 0);
    ASSERT(res_len == len);

    res_len = len;
    ASSERT(bk_str_read(str, res, &res_len) == 0);
    ASSERT(res_len == len);
    ASSERT(strcmp(res, val) == 0);
    ASSERT(res[res_len] == '\0');
}

static inline void test_str_printf_va(struct bk_str *str, const char *fmt, va_list ap) {
    ASSERT(bk_str_printf_va(NULL, fmt, ap) == -EINVAL);
    ASSERT(bk_str_printf_va(str, NULL, ap) == -EINVAL);
    ASSERT(bk_str_printf_va(str, fmt, NULL) == -EINVAL);

    bk_str_clear(str);
    bk_str_printf_va(str, fmt, ap);
    ASSERT(strcmp(bk_str_content(str), "abc123def456") == 0);

    /* the `test_str_printf_va()` is called and tested by `test_str_printf()`. */
}

static void test_str_printf(struct bk_str *str, const char *fmt, ...) {
    va_list ap;

    ASSERT(bk_str_printf(NULL, fmt) == -EINVAL);
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
    bk_str_write(str, val, len);
    ASSERT(strcmp(bk_str_content(str), val) == 0);
}

static inline void test_str_length(struct bk_str *str, const char *val, size_t len) {
    size_t res_len;

    ASSERT(bk_str_length(NULL, &res_len) == -EINVAL);
    ASSERT(bk_str_length(str, NULL) == -EINVAL);

    bk_str_clear(str);
    res_len = 10;
    ASSERT(bk_str_length(str, &res_len) == 0);
    ASSERT(res_len == 0);

    bk_str_write(str, val, len);
    ASSERT(bk_str_length(str, &res_len) == 0);
    ASSERT(res_len == len);
}

static inline void test_str_clear(struct bk_str *str, const char *val, size_t len) {
    size_t tmp_len;

    ASSERT(bk_str_clear(NULL) == -EINVAL);

    bk_str_clear(str);
    bk_str_write(str, val, len);
    ASSERT(bk_str_length(str, &tmp_len) == 0);
    ASSERT(tmp_len > 0);
    bk_str_clear(str);
    ASSERT(bk_str_length(str, &tmp_len) == 0);
    ASSERT(tmp_len == 0);
}

int main(void) {
    struct bk_str *str;
    const char *val = "abc123def456";
    const size_t len = strlen(val);

    str = bk_str_new();
    ASSERT(str);

    test_str_write(str, val, len);
    test_str_read(str, val, len);
    test_str_printf(str, "%s%d%s%d", "abc", 123, "def", 456);
    /* test_str_printf_va()` is already called by `test_str_printf`. */
    test_str_content(str, val, len);
    test_str_clear(str, val, len);
    test_str_length(str, val, len);

    bk_str_free(str);
    return EXIT_SUCCESS;
}