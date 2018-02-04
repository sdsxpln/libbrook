/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2018 Silvio Clecio, et al.
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

#include "b4r_assert.h"

#include <string.h>
#include "brook.h"

int main() {
    const char *ver_original;
    char ver_local[9];
    size_t ver_len;

    /* check version number */
    ASSERT(b4r_version() > 0);

    /* check version number string */
    sprintf(ver_local, "%d.%d.%d", B4R_VERSION_MAJOR, B4R_VERSION_MINOR, B4R_VERSION_PATCH);
    ver_original = b4r_version_string();
    ASSERT(strcmp(ver_original, ver_local) == 0);

    /* check version number string termination */
    ver_len = strlen(ver_original);
    ASSERT(ver_original[ver_len] == '\0');

    return EXIT_SUCCESS;
}