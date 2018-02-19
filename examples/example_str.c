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

/*
    the full output is:
    ./example_str
    Hello, world!
    abc123
*/

#include <stdio.h>
#include <stdlib.h>
#include <brook.h>

int main() {
    struct bk_str *str;

    str = bk_str_new(); /* create the string handle `str`. */

    bk_str_printf(str, "%s\n", "Hello, world!"); /* print "Hello, world!" to the handle. */
    printf("%s", bk_str_content(str)); /* get the printed content "hi" from `str` to the output screen. */
    bk_str_clear(str); /* clear all printed content in `str`. */

    bk_str_printf(str, "%s", "abc"); /* print "abc" to the handle. */
    bk_str_printf(str, "%d\n", 123); /* print "123" to the handle. */
    printf("%s", bk_str_content(str)); /* get the printed content "abc123" from `str` to the output screen. */

    bk_str_free(str); /* free the string handle `str`. */

    return EXIT_SUCCESS;
}