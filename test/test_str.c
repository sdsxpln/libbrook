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

static inline void check_str_write_raw(struct bk_str *str, const char *val, size_t len) {
    size_t tmp_len;

    ASSERT(bk_str_write_raw(NULL, val, len) == -EINVAL);
    ASSERT(bk_str_write_raw(str, NULL, len) == -EINVAL);
    ASSERT(bk_str_write_raw(str, val, 0) == -EINVAL);

    bk_str_clear(str);
    ASSERT(bk_str_write_raw(str, val, len) == 0);
    tmp_len = 0;
    bk_str_length(str, &tmp_len);
    ASSERT(tmp_len == len);
}

static inline void check_str_write(struct bk_str *str, const char *val, size_t len) {
    size_t tmp_len;

    ASSERT(bk_str_write(NULL, val) == -EINVAL);
    ASSERT(bk_str_write(str, NULL) == -EINVAL);

    bk_str_clear(str);
    ASSERT(bk_str_write(str, val) == 0);
    tmp_len = 0;
    bk_str_length(str, &tmp_len);
    ASSERT(tmp_len == len);
}

static inline void check_str_length(struct bk_str *str, const char *val, size_t len) {
    size_t tmp_len;

    ASSERT(bk_str_length(NULL, &tmp_len) == -EINVAL);
    ASSERT(bk_str_length(str, NULL) == -EINVAL);

    bk_str_clear(str);
    tmp_len = 123;
    ASSERT(bk_str_length(str, &tmp_len) == 0);
    ASSERT(tmp_len == 0);

    bk_str_write_raw(str, val, len);
    ASSERT(bk_str_length(str, &tmp_len) == 0);
    ASSERT(tmp_len == len);
}

static inline void check_str_clear(struct bk_str *str, const char *val, size_t len) {
    size_t tmp_len;

    ASSERT(bk_str_clear(NULL) == -EINVAL);

    bk_str_clear(str);
    bk_str_write_raw(str, val, len);
    ASSERT(bk_str_length(str, &tmp_len) == 0);
    ASSERT(tmp_len > 0);
    bk_str_clear(str);
    ASSERT(bk_str_length(str, &tmp_len) == 0);
    ASSERT(tmp_len == 0);
}

int main(void) {
    struct bk_str *str;
    const char *val = "abc123";
    const size_t len = strlen(val);

    str = bk_str_new();
    ASSERT(str);

    check_str_write_raw(str, val, len);
    check_str_write(str, val, len);
    check_str_length(str, val, len);
    check_str_clear(str, val, len);

    bk_str_free(str);
    return EXIT_SUCCESS;
}