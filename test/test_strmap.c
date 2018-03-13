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

extern int bk__strmap_new(struct bk_strmap **pair, const char *name, size_t name_len, const char *val, size_t val_len);

extern void bk__strmap_free(struct bk_strmap *pair);

static inline void test_strmap_name(void) {
    struct bk_strmap *pair = NULL;
    ASSERT(bk_strmap_name(pair) == NULL);
    ASSERT(bk_strmap_val(pair) == NULL);

    bk__strmap_new(&pair, "abc", strlen("abc"), "def", strlen("def"));
    ASSERT(pair);
    ASSERT(strcmp(bk_strmap_name(pair), "abc") == 0);
    ASSERT(strcmp(bk_strmap_val(pair), "def") == 0);

    bk__strmap_free(pair);
}

/*TODO: add more tests.*/

int main(void) {
    test_strmap_name();
    return EXIT_SUCCESS;
}
