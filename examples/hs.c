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

/*
    Simplest hash string example.

    Usage:

    $ ./hs

    It prints:

    S:  Hello!
    P:  Hello. How are you?
    S:  I'm fine. And you?
    P:  Me too.

    Chatters:
        P: Paim
        S: Silvio
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <brook.h>

static int hs_sort_cb(void *cls, struct b4r_hs *a, struct b4r_hs *b) {
    return strcmp(b4r_hs_val(b), b4r_hs_val(a)); /* desc */
}

static bool hs_iter_cb(void *cls, struct b4r_hs *hs) {
    const char *name = b4r_hs_name(hs);
    printf("\t%c: %s\n", *name, name);
    return true;
}

static void chat(struct b4r_hs **hsl, const char *name, const char *msg) {
    b4r_hs_add_or_set(hsl, name, msg);
    if (!b4r_is_empty(msg))
        printf("%c:\t%s\n", *name, b4r_hs_find_val(*hsl, name));
}

int main() {
    struct b4r_hs *hsl = NULL;
    chat(&hsl, "Silvio", "Hello!");
    chat(&hsl, "Paim", "..."); /* "..." will be replaced below... */
    chat(&hsl, "Paim", "Hello. How are you?");
    chat(&hsl, "Silvio", "I'm fine. And you?");
    chat(&hsl, "Paim", "Me too.");
    printf("\n");
    printf("Chatters: \n");
    b4r_hs_sort(&hsl, &hs_sort_cb, NULL);
    b4r_hs_iter(hsl, &hs_iter_cb, NULL);
    b4r_hs_cleanup(&hsl);
}