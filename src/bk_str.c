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

#include <errno.h>
#include "brook.h"
#include "bk_str.h"

struct bk_str *bk_str_new(void) {
    struct bk_str *str;
    str = bk_alloc(sizeof(struct bk_str));
    if (str)
        utstring_new(str->buf);
    return str;
}

void bk_str_free(struct bk_str *str) {
    if (!str)
        return;
    utstring_free(str->buf);
    bk_free(str);
}

int bk_str_write_raw(struct bk_str *str, const char *val, size_t len) {
    if (!str || !val || len == 0)
        return -EINVAL;
    utstring_bincpy(str->buf, val, len);
    return 0;
}

int bk_str_write(struct bk_str *str, const char *val) {
    if (!val)
        return -EINVAL;
    return bk_str_write_raw(str, val, strlen(val));
}

int bk_str_read_raw(struct bk_str *str, char *val, size_t *len) {
    if (!str || !val || !len || *len == 0)
        return -EINVAL;
    if (utstring_len(str->buf) >= *len) {
        *len = utstring_len(str->buf) + sizeof(char);
        return -ENOBUFS;
    }
    memcpy(val, utstring_body(str->buf), utstring_len(str->buf) + sizeof(char));
    *len = utstring_len(str->buf);
    return 0;
}

int bk_str_read(struct bk_str *str, char *val) {
    size_t len;
    if (!val)
        return -EINVAL;
    len = strlen(val);
    return bk_str_read_raw(str, val, &len);
}

const char *bk_str_content(struct bk_str *str) {
    if (!str)
        return NULL;
    return utstring_body(str->buf);
}

int bk_str_length(struct bk_str *str, size_t *len) {
    if (!str || !len)
        return -EINVAL;
    *len = utstring_len(str->buf);
    return 0;
}

int bk_str_clear(struct bk_str *str) {
    if (!str)
        return -EINVAL;
    utstring_clear(str->buf);
    return 0;
}