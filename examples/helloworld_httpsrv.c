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
    Simplest 'Hello world' HTTP server example.

    Usage:

    Start the server in a terminal:

    $ ./helloworld_httpsrv 8080

    Now, in other terminal, call the client:

    $ curl http://localhost:8080

    or

    $ wget -qO- http://localhost:8080

    or just open http://localhost:8080 on your browser.
*/

#include <stdlib.h>
#include <stdio.h>
#include <brook.h>

#define USAGE_FMT "Usage: %s <port>\n"

static void srv_req_cb(void *cls, struct b4r_httpsrv_req *req, struct b4r_httpsrv_res *res, bool *done) {
    b4r_httpsrv_res_send(res, "<html><body>Hello world</body></html>");
}

int main(int argc, const char **argv) {
    struct b4r_httpsrv_cfg *cfg;
    struct b4r_httpsrv *srv;
    uint16_t port;
    bool success;
    if (argc != 2) {
        fprintf(stderr, USAGE_FMT, argv[0]);
        return EXIT_FAILURE;
    }
    if (!(cfg = b4r_httpsrv_cfg_new(NULL))) {
        fprintf(stderr, "Failed creating configuration.\n");
        return EXIT_FAILURE;
    }
    if (!(srv = b4r_httpsrv_new(cfg, &srv_req_cb, NULL, NULL, NULL, NULL, NULL))) {
        b4r_httpsrv_cfg_free(cfg);
        fprintf(stderr, "Failed creating server.\n");
        return EXIT_FAILURE;
    }
    port = (uint16_t) strtol(argv[1], NULL, 10);
    b4r_httpsrv_setopt(srv, B4R_HTTPSRV_OPT_PORT, port);
    if ((success = b4r_httpsrv_listen(srv))) {
        fprintf(stdout, "Server listening at http://localhost:%d/\n", port);
        getchar();
        b4r_httpsrv_shutdown(srv);
    } else
        fprintf(stderr, "Listen to http://localhost:%d failed.\n", port);
    b4r_httpsrv_free(srv);
    b4r_httpsrv_cfg_free(cfg);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}