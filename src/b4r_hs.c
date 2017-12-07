/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2017 Silvio Clecio, et al.
 *
 * This file is part of Brook4-REST.
 *
 * Brook4-REST is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brook4-REST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Brook4-REST.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include "b4r_hs.h"
#include "b4r_macros.h"
#include <brook.h>

#define _B4R_HS_NEW_KEY(name) !b4r_is_empty(name) ? b4r_lower(b4r_dup(name)) : NULL

#define _B4R_HS_FIND(hsl, name, key, hs) do { \
    *(key) = _B4R_HS_NEW_KEY(name); \
    if ((hsl) && *(key)) \
        HASH_FIND_STR(hsl, *(key), *(hs)); \
    else \
        *(hs) = NULL; \
} while (0)

#define _B4R_HS_DEL(hsl, hs) do { \
    HASH_DEL(*(hsl), hs); \
    _b4r_hs_free(hs); \
} while (0)

static struct b4r_hs *_b4r_hs_new(char *key, const char *name, const char *val) {
    struct b4r_hs *hs;
    _B4R_NEW(&hs);
    hs->key = key;
    if (!(hs->name = b4r_dup(name))) {
        _B4R_FREE(hs->key);
        _B4R_FREE(hs);
        return NULL;
    }
    if (!(hs->val = b4r_dup(val))) {
        _B4R_FREE(hs->key);
        _B4R_FREE(hs->name);
        _B4R_FREE(hs);
        return NULL;
    }
    return hs;
}

static void _b4r_hs_free(struct b4r_hs *hs) {
    if (hs) {
        _B4R_FREE(hs->key);
        _B4R_FREE(hs->name);
        _B4R_FREE(hs->val);
        _B4R_FREE(hs);
    }
}

const char *b4r_hs_name(struct b4r_hs *hs) {
    return hs ? hs->name : NULL;
}

const char *b4r_hs_val(struct b4r_hs *hs) {
    return hs ? hs->val : NULL;
}

bool b4r_hs_name_val(struct b4r_hs *hs, const char **name, const char **val) {
    if (!hs)
        return false;
    *name = hs->name;
    *val = hs->val;
    return true;
}

bool b4r_hs_add(struct b4r_hs **hsl, const char *name, const char *val) {
    struct b4r_hs *hs;
    char *key;
    if (!(key = _B4R_HS_NEW_KEY(name)))
        return false;
    if (!(hs = _b4r_hs_new(key, name, val))) {
        _B4R_FREE(key);
        return false;
    }
    HASH_ADD_STR(*hsl, name, hs);
    return true;
}

bool b4r_hs_set(struct b4r_hs **hsl, const char *name, const char *val) {
    struct b4r_hs *hs;
    char *key;
    if (!hsl)
        return false;
    _B4R_HS_FIND(*hsl, name, &key, &hs);
    if (!key)
        return false;
    if (!hs) {
        if (!(hs = _b4r_hs_new(key, name, val))) {
            _B4R_FREE(key);
            return false;
        }
        HASH_ADD_STR(*hsl, key, hs);
        return true;
    }
    _B4R_FREE(key);
    _B4R_FREE(hs->val);
    if (!(hs->val = b4r_dup(val))) {
        _B4R_HS_DEL(hsl, hs);
        return false;
    }
    return true;
}

bool b4r_hs_rm(struct b4r_hs **hsl, const char *name) {
    struct b4r_hs *hs;
    char *key;
    if (!hsl)
        return false;
    _B4R_HS_FIND(*hsl, name, &key, &hs);
    if (!key)
        return false;
    _B4R_FREE(key);
    if (hs) {
        _B4R_HS_DEL(hsl, hs);
        return true;
    }
    return false;
}

struct b4r_hs *b4r_hs_find(struct b4r_hs *hsl, const char *name) {
    struct b4r_hs *hs;
    char *key;
    _B4R_HS_FIND(hsl, name, &key, &hs);
    if (!key)
        return NULL;
    _B4R_FREE(key);
    return hs;
}

const char *b4r_hs_get(struct b4r_hs *hsl, const char *name) {
    struct b4r_hs *hs;
    hs = b4r_hs_find(hsl, name);
    return hs ? hs->val : NULL;
}

bool b4r_hs_try(struct b4r_hs *hsl, const char *name, const char **val) {
    struct b4r_hs *hs;
    char *key;
    _B4R_HS_FIND(hsl, name, &key, &hs);
    if (!key)
        return false;
    _B4R_FREE(key);
    if (hs) {
        *val = hs->val;
        return true;
    }
    return false;
}

bool b4r_hs_has(struct b4r_hs *hsl, const char *name) {
    struct b4r_hs *hs;
    char *key;
    _B4R_HS_FIND(hsl, name, &key, &hs);
    if (!key)
        return false;
    _B4R_FREE(key);
    return (NULL != hs);
}

bool b4r_hs_iter(struct b4r_hs *hsl, b4r_hs_iter_cb iter_cb, void *iter_cls) {
    struct b4r_hs *hs, *tmp;
    if (!hsl || !iter_cb)
        return false;
    HASH_ITER(hh, hsl, hs, tmp) {
        if (!iter_cb(iter_cls, hs))
            break;
    }
    return true;
}

bool b4r_hs_next(struct b4r_hs **hs) {
    return hs && (*hs = (*hs ? (*hs)->hh.next : NULL));
}

unsigned int b4r_hs_count(struct b4r_hs *hsl) {
    return HASH_COUNT(hsl);
}

bool b4r_hs_sort(struct b4r_hs **hsl, b4r_hs_sort_cb cmp_cb, void *cmp_cls) {
    if (!hsl || !cmp_cb)
        return false;
#define CMP(a, b) cmp_cb(cmp_cls, a, b)
    HASH_SORT(*hsl, CMP);
#undef CMP
    return true;
}

void b4r_hs_cleanup(struct b4r_hs **hsl) {
    struct b4r_hs *hs, *tmp;
    if (hsl && *hsl) {
        HASH_ITER(hh, *hsl, hs, tmp) {
            _B4R_HS_DEL(hsl, hs);
        }
        *hsl = NULL;
    }
}

#undef _B4R_HS_NEW_KEY

#undef _B4R_HS_FIND

#undef _B4R_HS_DEL