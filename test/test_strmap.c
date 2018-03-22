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
    ASSERT(bk_strmap_add(NULL, name, val) == -EINVAL);
    ASSERT(bk_strmap_add(map, NULL, val) == -EINVAL);
    ASSERT(bk_strmap_add(map, name, NULL) == -EINVAL);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    ASSERT(bk_strmap_add(map, "", val) == 0);
    ASSERT(bk_strmap_count(*map) == 1);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_add(map, name, "") == 0);
    ASSERT(bk_strmap_count(*map) == 1);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_add(map, name, val) == 0);
    ASSERT(bk_strmap_add(map, name, val) == 0);
    ASSERT(bk_strmap_count(*map) == 2);
}

static inline void test_strmap_set(struct bk_strmap **map, const char *name, const char *val) {
    ASSERT(bk_strmap_set(NULL, name, val) == -EINVAL);
    ASSERT(bk_strmap_set(map, NULL, val) == -EINVAL);
    ASSERT(bk_strmap_set(map, name, NULL) == -EINVAL);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    ASSERT(bk_strmap_set(map, "", val) == 0);
    ASSERT(bk_strmap_count(*map) == 1);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_set(map, name, "") == 0);
    ASSERT(bk_strmap_count(*map) == 1);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_set(map, name, val) == 0);
    ASSERT(bk_strmap_set(map, name, val) == 0);
    ASSERT(bk_strmap_count(*map) == 1);
}

static inline void test_strmap_find(struct bk_strmap **map, const char *name, const char *val) {
    struct bk_strmap *pair;
    ASSERT(bk_strmap_find(NULL, name, &pair) == -EINVAL);
    ASSERT(bk_strmap_find(*map, NULL, &pair) == -EINVAL);
    ASSERT(bk_strmap_find(*map, name, NULL) == -EINVAL);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    bk_strmap_add(map, name, val);
    ASSERT(bk_strmap_count(*map) == 1);
    ASSERT(bk_strmap_find(*map, "", &pair) == -ENOENT);
    ASSERT(!pair);
    ASSERT(bk_strmap_find(*map, "xxx", &pair) == -ENOENT);
    ASSERT(!pair);
    ASSERT(bk_strmap_find(*map, "yyy", &pair) == -ENOENT);
    ASSERT(!pair);

    bk_strmap_add(map, "", "");
    bk_strmap_add(map, "xxx", "yyy");
    bk_strmap_add(map, "yyy", "xxx");
    ASSERT(bk_strmap_count(*map) == 4);
    ASSERT(bk_strmap_find(*map, name, &pair) == 0);
    ASSERT(pair);
    ASSERT(strcmp(bk_strmap_name(pair), name) == 0 && strcmp(bk_strmap_val(pair), val) == 0);
    ASSERT(bk_strmap_find(*map, "", &pair) == 0);
    ASSERT(pair);
    ASSERT(strcmp(bk_strmap_name(pair), "") == 0 && strcmp(bk_strmap_val(pair), "") == 0);
    ASSERT(bk_strmap_find(*map, "xxx", &pair) == 0);
    ASSERT(pair);
    ASSERT(strcmp(bk_strmap_name(pair), "xxx") == 0 && strcmp(bk_strmap_val(pair), "yyy") == 0);
    ASSERT(bk_strmap_find(*map, "yyy", &pair) == 0);
    ASSERT(pair);
    ASSERT(strcmp(bk_strmap_name(pair), "yyy") == 0 && strcmp(bk_strmap_val(pair), "xxx") == 0);
}

static inline void test_strmap_rm(struct bk_strmap **map, const char *name, const char *val) {
    struct bk_strmap *pair;
    ASSERT(bk_strmap_rm(NULL, name) == -EINVAL);
    ASSERT(bk_strmap_rm(map, NULL) == -EINVAL);

    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    bk_strmap_add(map, name, val);
    ASSERT(bk_strmap_count(*map) == 1);
    ASSERT(bk_strmap_rm(map, "") == -ENOENT);
    ASSERT(bk_strmap_count(*map) == 1);
    ASSERT(bk_strmap_rm(map, "xxx") == -ENOENT);
    ASSERT(bk_strmap_count(*map) == 1);
    ASSERT(bk_strmap_rm(map, "yyy") == -ENOENT);
    ASSERT(bk_strmap_count(*map) == 1);

    bk_strmap_add(map, "", "");
    bk_strmap_add(map, "xxx", "yyy");
    bk_strmap_add(map, "yyy", "xxx");
    ASSERT(bk_strmap_count(*map) == 4);

    ASSERT(bk_strmap_find(*map, name, &pair) == 0);
    ASSERT(bk_strmap_rm(map, name) == 0);
    ASSERT(bk_strmap_count(*map) == 3);
    ASSERT(bk_strmap_find(*map, name, &pair) == -ENOENT);

    ASSERT(bk_strmap_find(*map, "", &pair) == 0);
    ASSERT(bk_strmap_rm(map, "") == 0);
    ASSERT(bk_strmap_count(*map) == 2);
    ASSERT(bk_strmap_find(*map, "", &pair) == -ENOENT);

    ASSERT(bk_strmap_find(*map, "xxx", &pair) == 0);
    ASSERT(bk_strmap_rm(map, "xxx") == 0);
    ASSERT(bk_strmap_count(*map) == 1);
    ASSERT(bk_strmap_find(*map, "xxx", &pair) == -ENOENT);

    ASSERT(bk_strmap_find(*map, "yyy", &pair) == 0);
    ASSERT(bk_strmap_rm(map, "yyy") == 0);
    ASSERT(bk_strmap_count(*map) == 0);
    ASSERT(bk_strmap_find(*map, "yyy", &pair) == -EINVAL);
}

static int strmap_iter_empty(void *cls, struct bk_strmap *pair) {
    (void) cls;
    (void) pair;
    return 0;
}

static int strmap_iter_123(void *cls, struct bk_strmap *pair) {
    (void) cls;
    (void) pair;
    return 123;
}

static int strmap_iter_concat(void *cls, struct bk_strmap *pair) {
    strcat(cls, bk_strmap_name(pair));
    strcat(cls, bk_strmap_val(pair));
    return 0;
}

static inline void test_strmap_iter(struct bk_strmap **map) {
    char str[100];
    ASSERT(bk_strmap_iter(NULL, strmap_iter_empty, "") == -EINVAL);
    ASSERT(bk_strmap_iter(*map, NULL, "") == -EINVAL);

    bk_strmap_cleanup(map);
    bk_strmap_add(map, "abc", "123");
    bk_strmap_add(map, "def", "456");

    ASSERT(bk_strmap_iter(*map, strmap_iter_empty, NULL) == 0);
    ASSERT(bk_strmap_iter(*map, strmap_iter_123, NULL) == 123);

    memset(str, 0, sizeof(str));
    ASSERT(strcmp(str, "") == 0);
    ASSERT(bk_strmap_iter(*map, strmap_iter_concat, str) == 0);
    ASSERT(strcmp(str, "abc123def456") == 0);
}

static int strmap_sort_empty(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b) {
#ifdef __ANDROID__
    sprintf(cls, "%s%s", (char *) cls, (char *) cls);
#else
    strcat(cls, cls);
#endif
    (void) pair_a;
    (void) pair_b;
    return 0;
}

static int strmap_sort_name_desc(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b) {
    (void) cls;
    return strcmp(bk_strmap_name(pair_b), bk_strmap_name(pair_a));
}

static int strmap_sort_name_asc(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b) {
    (void) cls;
    return strcmp(bk_strmap_name(pair_a), bk_strmap_name(pair_b));
}

static int strmap_sort_val_desc(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b) {
    (void) cls;
    return strcmp(bk_strmap_val(pair_b), bk_strmap_val(pair_a));
}

static int strmap_sort_val_asc(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b) {
    (void) cls;
    return strcmp(bk_strmap_val(pair_a), bk_strmap_val(pair_b));
}

static inline void test_strmap_sort(struct bk_strmap **map) {
    char str[100];
    ASSERT(bk_strmap_sort(NULL, strmap_sort_empty, "") == -EINVAL);
    ASSERT(bk_strmap_sort(map, NULL, "") == -EINVAL);

    bk_strmap_cleanup(map);
    bk_strmap_add(map, "abc", "123");
    bk_strmap_add(map, "def", "456");

    memset(str, 0, sizeof(str));
    strcpy(str, "abc");
    ASSERT(strcmp(str, "abc") == 0);
    ASSERT(bk_strmap_sort(map, strmap_sort_empty, str) == 0);
    ASSERT(strcmp(str, "abcabc") == 0);

    memset(str, 0, sizeof(str));
    ASSERT(strcmp(str, "") == 0);
    bk_strmap_iter(*map, strmap_iter_concat, str);
    ASSERT(strcmp(str, "abc123def456") == 0);
    ASSERT(bk_strmap_sort(map, strmap_sort_name_desc, NULL) == 0);
    memset(str, 0, sizeof(str));
    bk_strmap_iter(*map, strmap_iter_concat, str);
    ASSERT(strcmp(str, "def456abc123") == 0);

    ASSERT(bk_strmap_sort(map, strmap_sort_name_asc, NULL) == 0);
    memset(str, 0, sizeof(str));
    bk_strmap_iter(*map, strmap_iter_concat, str);
    ASSERT(strcmp(str, "abc123def456") == 0);

    ASSERT(bk_strmap_sort(map, strmap_sort_val_desc, NULL) == 0);
    memset(str, 0, sizeof(str));
    bk_strmap_iter(*map, strmap_iter_concat, str);
    ASSERT(strcmp(str, "def456abc123") == 0);

    ASSERT(bk_strmap_sort(map, strmap_sort_val_asc, NULL) == 0);
    memset(str, 0, sizeof(str));
    bk_strmap_iter(*map, strmap_iter_concat, str);
    ASSERT(strcmp(str, "abc123def456") == 0);
}

static inline void test_strmap_count(struct bk_strmap **map, const char *name, const char *val) {
    ASSERT(bk_strmap_count(NULL) == 0);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    bk_strmap_add(map, name, val);
    ASSERT(bk_strmap_count(*map) == 1);
    bk_strmap_add(map, "xxx", "yyy");
    ASSERT(bk_strmap_count(*map) == 2);
    bk_strmap_add(map, "yyy", "xxx");
    ASSERT(bk_strmap_count(*map) == 3);
    bk_strmap_add(map, name, val);
    ASSERT(bk_strmap_count(*map) == 4);
    bk_strmap_rm(map, name);
    ASSERT(bk_strmap_count(*map) == 3);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
}

static inline void test_strmap_next(struct bk_strmap **map) {
    struct bk_strmap *pair;
    char str[100];
    ASSERT(bk_strmap_next(NULL) == -EINVAL);
    bk_strmap_cleanup(map);
    pair = NULL;
    ASSERT(bk_strmap_next(&pair) == 0);
    ASSERT(!pair);
    bk_strmap_add(map, "abc", "123");
    bk_strmap_add(map, "def", "456");
    bk_strmap_add(map, "xxx", "yyy");
    memset(str, 0, sizeof(str));
    ASSERT(strcmp(str, "") == 0);
    pair = *map;
    while (pair) {
        strcat(str, bk_strmap_name(pair));
        strcat(str, bk_strmap_val(pair));
        bk_strmap_next(&pair);
    }
    ASSERT(strcmp(str, "abc123def456xxxyyy") == 0);
}

static inline void test_strmap_cleanup(struct bk_strmap **map) {
    bk_strmap_cleanup(NULL); /* try to raise some error */
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    bk_strmap_add(map, "abc", "123");
    bk_strmap_add(map, "def", "456");
    bk_strmap_add(map, "xxx", "yyy");
    ASSERT(bk_strmap_count(*map) == 3);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
    bk_strmap_cleanup(map);
    ASSERT(bk_strmap_count(*map) == 0);
}

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
    test_strmap_set(&map, name, val);
    test_strmap_find(&map, name, val);
    test_strmap_rm(&map, name, val);
    test_strmap_iter(&map);
    test_strmap_sort(&map);
    test_strmap_count(&map, name, val);
    test_strmap_next(&map);
    test_strmap_cleanup(&map);

    bk_strmap_cleanup(&map);
    return EXIT_SUCCESS;
}
