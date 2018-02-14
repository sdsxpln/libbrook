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

int main(void) {
    const char *data = "abc123";
    const size_t data_len = strlen(data);
    size_t len;
    struct bk_str *str;

    /* Checks if a string object is successfully created. */
    str = bk_str_new();
    ASSERT(str);

    /* Checks if the functions returns -EINVAL giving NULL parameters. */
    ASSERT(bk_str_write_raw(NULL, data, data_len) == -EINVAL);
    ASSERT(bk_str_write_raw(str, NULL, data_len) == -EINVAL);
    ASSERT(bk_str_write_raw(str, data, 0) == -EINVAL);

    ASSERT(bk_str_write(NULL, data) == -EINVAL);
    ASSERT(bk_str_write(str, NULL) == -EINVAL);

    ASSERT(bk_str_length(NULL, &len) == -EINVAL);
    ASSERT(bk_str_length(str, NULL) == -EINVAL);

    ASSERT(bk_str_clear(NULL) == -EINVAL);

    /* There is no a portable way to test if a memory is freed, so just free it. */
    bk_str_free(str);
    return EXIT_SUCCESS;
}