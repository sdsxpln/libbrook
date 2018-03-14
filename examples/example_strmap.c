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
    C:	Hello!
    P:	Hello. How are you?
    C:	I'm fine. And you?
    P:	Me too.

    Chatters:
	    P: Paim
	    C: Clecio
 */

/* NOTE: Error checking has been omitted for clarity. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <brook.h>

static int map_sort(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b) {
    (void) cls;
    return strcmp(bk_strmap_val(pair_b), bk_strmap_val(pair_a)); /* desc */
}

static int map_iter(void *cls, struct bk_strmap *pair) {
    const char *name = bk_strmap_name(pair);
    (void) cls;
    printf("\t%c: %s\n", *name, name);
    return 0;
}

static void chat(struct bk_strmap **map, const char *name, const char *msg) {
    struct bk_strmap *pair;
    bk_strmap_set(map, name, msg);
    if (msg && (bk_strmap_find(*map, name, &pair) == 0))
        printf("%c:\t%s\n", *name, bk_strmap_val(pair));
}

int main(void) {
    struct bk_strmap *map = NULL;
    chat(&map, "Clecio", "Hello!");
    chat(&map, "Paim", "Hello. How are you?");
    chat(&map, "Clecio", "I'm fine. And you?");
    chat(&map, "Paim", "Me too.");
    printf("\nChatters:\n");
    bk_strmap_sort(&map, &map_sort, NULL);
    bk_strmap_iter(map, &map_iter, NULL);
    bk_strmap_cleanup(&map);
    return EXIT_SUCCESS;
}
