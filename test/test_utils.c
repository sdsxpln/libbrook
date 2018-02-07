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

#include "b4r_assert.h"

#include <string.h>
#include "brook.h"

int main(void) {
    const char *ver_original;
    char ver_local[9];
    size_t ver_len;

    /* Checks if the version number is greater than zero */
    ASSERT(b4r_version() > 0);

    /* Checks if the stringified version is a valid string */
    ver_original = b4r_version_string();
    ASSERT(ver_original);
    ver_len = strlen(ver_original);
    ASSERT(ver_len > 0);

    /* Checks if the stringified version matches the library version */
    sprintf(ver_local, "%d.%d.%d", B4R_VERSION_MAJOR, B4R_VERSION_MINOR, B4R_VERSION_PATCH);
    ASSERT(strcmp(ver_original, ver_local) == 0);

    /* Checks if the stringified version has a null termination */
    ASSERT(ver_original[ver_len] == '\0');

    return EXIT_SUCCESS;
}