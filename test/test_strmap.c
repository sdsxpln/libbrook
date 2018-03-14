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
    ASSERT(strcmp(bk_strmap_name(pair), "abç") == 0);
}

static inline void test_strmap_val(struct bk_strmap *pair) {
    ASSERT(bk_strmap_val(NULL) == NULL);
    ASSERT(strcmp(bk_strmap_val(pair), "déf") == 0);
}

static inline void test_strmap_add(struct bk_strmap **map, const char *name, const char *val) {
    unsigned int count;

    ASSERT(bk_strmap_add(NULL, name, val) == -EINVAL);
    ASSERT(bk_strmap_add(map, NULL, val) == -EINVAL);
    ASSERT(bk_strmap_add(map, name, NULL) == -EINVAL);

    bk_strmap_cleanup(map);
    bk_strmap_count(*map, &count);
    ASSERT(count == 0);

    ASSERT(bk_strmap_add(map, "", val) == 0);
    bk_strmap_count(*map, &count);
    ASSERT(count == 1);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_add(map, name, "") == 0);
    bk_strmap_count(*map, &count);
    ASSERT(count == 1);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_add(map, name, val) == 0);
    ASSERT(bk_strmap_add(map, name, val) == 0);
    bk_strmap_count(*map, &count);
    ASSERT(count == 2);
}

/*TODO: add more tests.*/

int main(void) {
    struct bk_strmap *map = NULL, *pair;
    char name[] = "abç";
    char val[] = "déf";

    bk_strmap_add(&map, name, val);
    bk_strmap_find(map, name, &pair);
    ASSERT(pair);

    test_strmap_name(pair);
    test_strmap_val(pair);
    test_strmap_add(&map, name, val);

    bk_strmap_cleanup(&map);

    return EXIT_SUCCESS;
}
