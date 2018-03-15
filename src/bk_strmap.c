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
#include "bk_macros.h"
#include "brook.h"
#include "bk_utils.h"
#include "bk_strmap.h"

static inline void bk__strmap_cleanup(struct bk_strmap *pair) {
    if (!pair)
        return;
    bk_free(pair->key);
    bk_free(pair->name);
    bk_free(pair->val);
    bk_free(pair);
}

const char *bk_strmap_name(struct bk_strmap *pair) {
    return pair ? pair->name : NULL;
}

const char *bk_strmap_val(struct bk_strmap *pair) {
    return pair ? pair->val : NULL;
}

int bk_strmap_add(struct bk_strmap **map, const char *name, const char *val) {
    struct bk_strmap *pair;
    if (!map || !name || !val)
        return -EINVAL;
    pair = bk_alloc(sizeof(struct bk_strmap));
    if (!pair)
        oom();
    pair->key = strdup(name);
    pair->name = strdup(name);
    pair->val = strdup(val);
    if (!pair->key || !pair->name || !pair->val) {
        bk__strmap_cleanup(pair);
        oom();
    }
    bk__toasciilower(pair->key);
    HASH_ADD_STR(*map, key, pair);
    return 0;
}

int bk_strmap_set(struct bk_strmap **map, const char *name, const char *val) {
    struct bk_strmap *pair, *tmp;
    if (!map || !name || !val)
        return -EINVAL;
    pair = bk_alloc(sizeof(struct bk_strmap));
    if (!pair)
        oom();
    pair->key = strdup(name);
    pair->name = strdup(name);
    pair->val = strdup(val);
    if (!pair->key || !pair->name || !pair->val) {
        bk__strmap_cleanup(pair);
        oom();
    }
    bk__toasciilower(pair->key);
    HASH_REPLACE_STR(*map, key, pair, tmp);
    bk__strmap_cleanup(tmp);
    return 0;
}

int bk_strmap_find(struct bk_strmap *map, const char *name, struct bk_strmap **pair) {
    char *key;
    if (!map || !pair || !name)
        return -EINVAL;
    key = strdup(name);
    if (!key)
        oom();
    bk__toasciilower(key);
    HASH_FIND_STR(map, key, *pair);
    if (!*pair) {
        bk_free(key);
        return -ENOENT;
    }
    bk_free(key);
    return 0;
}

int bk_strmap_rm(struct bk_strmap **map, const char *name) {
    struct bk_strmap *pair;
    int ret = bk_strmap_find(*map, name, &pair);
    if (ret != 0)
        return ret;
    HASH_DELETE_HH(hh, *map, &pair->hh);
    bk__strmap_cleanup(pair);
    return 0;
}

int bk_strmap_iter(struct bk_strmap *map, bk_strmap_iter_cb iter_cb, void *iter_cls) {
    struct bk_strmap *pair, *tmp;
    int ret;
    if (!map || !iter_cb)
        return -EINVAL;
    HASH_ITER(hh, map, pair, tmp) {
        if ((ret = iter_cb(iter_cls, pair)) != 0)
            return ret;
    }
    return 0;
}

int bk_strmap_sort(struct bk_strmap **map, bk_strmap_sort_cb cmp_cb, void *cmp_cls) {
    if (!map || !cmp_cb)
        return -EINVAL;
#define _BK_CMP(a, b) cmp_cb(cmp_cls, a, b)
    HASH_SORT(*map, _BK_CMP);
#undef _BK_CMP
    return 0;
}

unsigned int bk_strmap_count(struct bk_strmap *map) {
    return HASH_COUNT(map);
}

int bk_strmap_next(struct bk_strmap **map) {
    if (!map)
        return -EINVAL;
    *map = *map ? (*map)->hh.next : NULL;
    return 0;
}

void bk_strmap_cleanup(struct bk_strmap **map) {
    struct bk_strmap *pair, *tmp;
    if (map && *map) {
        HASH_ITER(hh, *map, pair, tmp) {
            HASH_DEL(*map, pair);
            bk__strmap_cleanup(pair);
        }
        *map = NULL;
    }
}
