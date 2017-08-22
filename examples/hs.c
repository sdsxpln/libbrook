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
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://brookframework.org/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
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