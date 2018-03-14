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
#include <brook.h>

static inline void test_strmap_name(struct bk_strmap *pair) {
    ASSERT(bk_strmap_name(NULL) == NULL);
    ASSERT(strcmp(bk_strmap_name(pair), "abc") == 0);
}

static inline void test_strmap_val(struct bk_strmap *pair) {
    ASSERT(bk_strmap_val(NULL) == NULL);
    ASSERT(strcmp(bk_strmap_val(pair), "def") == 0);
}

static inline void test_strmap_readname(struct bk_strmap *pair, const char *name, size_t name_len) {
    char res[16 * sizeof(char)];
    size_t res_len;

    ASSERT(bk_strmap_readname(NULL, res, &res_len) == -EINVAL);
    ASSERT(bk_strmap_readname(pair, NULL, &res_len) == -EINVAL);
    ASSERT(bk_strmap_readname(pair, res, NULL) == -EINVAL);
    ASSERT(bk_strmap_readname(pair, res, (size_t *) 0) == -EINVAL);

    res_len = sizeof(char);
    ASSERT(bk_strmap_readname(pair, res, &res_len) == -ENOBUFS);
    ASSERT(bk_strmap_readname(pair, res, &res_len) == 0);
    ASSERT(res_len == name_len);
    ASSERT(memcmp(name, res, sizeof(char)) == 0);

    res_len = name_len + name_len;
    ASSERT(bk_strmap_readname(pair, res, &res_len) == 0);
    ASSERT(res_len == name_len);

    res_len = name_len + sizeof(char);
    ASSERT(bk_strmap_readname(pair, res, &res_len) == 0);
    ASSERT(res_len == name_len);
    ASSERT(strcmp(res, name) == 0);
    ASSERT(res[res_len] == '\0');
}

static inline void test_strmap_readval(struct bk_strmap *pair, const char *val, size_t val_len) {
    char res[16 * sizeof(char)];
    size_t res_len;

    ASSERT(bk_strmap_readval(NULL, res, &res_len) == -EINVAL);
    ASSERT(bk_strmap_readval(pair, NULL, &res_len) == -EINVAL);
    ASSERT(bk_strmap_readval(pair, res, NULL) == -EINVAL);
    ASSERT(bk_strmap_readval(pair, res, (size_t *) 0) == -EINVAL);

    res_len = sizeof(char);
    ASSERT(bk_strmap_readval(pair, res, &res_len) == -ENOBUFS);
    ASSERT(bk_strmap_readval(pair, res, &res_len) == 0);
    ASSERT(res_len == val_len);
    ASSERT(memcmp(val, res, sizeof(char)) == 0);

    res_len = val_len + val_len;
    ASSERT(bk_strmap_readval(pair, res, &res_len) == 0);
    ASSERT(res_len == val_len);

    res_len = val_len + sizeof(char);
    ASSERT(bk_strmap_readval(pair, res, &res_len) == 0);
    ASSERT(res_len == val_len);
    ASSERT(strcmp(res, val) == 0);
    ASSERT(res[res_len] == '\0');
}

static inline void test_strmap_add(struct bk_strmap **map, const char *name, size_t name_len, const char *val,
                                   size_t val_len) {
    ASSERT(bk_strmap_add(map, name, name_len, val, val_len) == 0);
    ASSERT(bk_strmap_add(NULL, name, name_len, val, val_len) == -EINVAL);
    ASSERT(bk_strmap_add(map, NULL, name_len, val, val_len) == -EINVAL);
    ASSERT(bk_strmap_add(map, name, 0, val, val_len) == -EINVAL);
    ASSERT(bk_strmap_add(map, name, name_len, NULL, val_len) == -EINVAL);
    ASSERT(bk_strmap_add(map, name, name_len, val, 0) == -EINVAL);

    bk_strmap_cleanup(map);
    ASSERT(!*map);
}

/*TODO: add more tests.*/

int main(void) {
    struct bk_strmap *map = NULL, *pair;
    char name[] = "abc";
    char val[] = "def";
    size_t name_len = strlen(name);
    size_t val_len = strlen(val);

    bk_strmap_add(&map, name, name_len, val, val_len);
    bk_strmap_find(map, name, name_len, &pair);
    ASSERT(pair);

    test_strmap_name(pair);
    test_strmap_val(pair);
    test_strmap_readname(pair, name, name_len);
    test_strmap_readval(pair, val, val_len);
    test_strmap_add(&map, name, name_len, val, val_len);

    bk_strmap_cleanup(&map);

    return EXIT_SUCCESS;
}
