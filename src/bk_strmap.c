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
#include "bk_utils.h"
#include "bk_strmap.h"

static void bk__strmap_cleanup(struct bk_strmap *pair) {
    if (!pair)
        return;
    bk_free(pair->key);
    bk_free(pair->name);
    bk_free(pair->val);
    bk_free(pair);
}

int bk_strmap_name(struct bk_strmap *pair, char *name, size_t *len) {
    if (!pair || !name || !len || *len == 0)
        return -EINVAL;
    if (*len < pair->name_len)
        return -ENOBUFS;
    if (*len > pair->name_len)
        *len = pair->name_len;
    memcpy(name, pair->name, *len);
    name[*len] = '\0';
    return 0;
}

int bk_strmap_val(struct bk_strmap *pair, char *val, size_t *len) {
    if (!pair || !val || !len || *len == 0)
        return -EINVAL;
    if (*len < pair->val_len)
        return -ENOBUFS;
    if (*len > pair->val_len)
        *len = pair->val_len;
    memcpy(val, pair->val, *len);
    val[*len] = '\0';
    return 0;
}

int bk_strmap_add(struct bk_strmap **map, const char *name, size_t name_len, const char *val, size_t val_len) {
    struct bk_strmap *pair;
    int ret;
    if (!map || !name || name_len == 0 || !val || val_len == 0)
        return -EINVAL;
    pair = bk_alloc(sizeof(struct bk_strmap));
    if (!pair)
        return -ENOMEM;
    pair->name = bk__strndup(name, name_len);
    pair->name_len = name_len;
    pair->val = bk__strndup(val, val_len);
    pair->val_len = val_len;
    if (!pair->name || !pair->val) {
        bk__strmap_cleanup(pair);
        return -ENOMEM;
    }
    pair->key = bk__strndup(pair->name, pair->name_len);
    if ((ret = bk__toasciilower(pair->key, pair->name_len)) != 0) {
        bk__strmap_cleanup(pair);
        return ret;
    }
    HASH_ADD_KEYPTR(hh, *map, pair->key, name_len, pair);
    return 0;
}

int bk_strmap_find(struct bk_strmap *map, const char *name, size_t len, struct bk_strmap **pair) {
    char *key;
    int ret;
    if (!map || !pair)
        return -EINVAL;
    key = bk__strndup(name, len);
    if ((ret = bk__toasciilower(key, len)) == 0) {
        HASH_FIND(hh, map, key, len, *pair);
        if (!*pair)
            ret = -ENOENT;
    }
    bk_free(key);
    return ret;
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
