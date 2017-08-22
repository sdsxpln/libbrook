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
    Simplest uploads HTTP server example.

    Usage:

    Start the server in a terminal:

    $ ./httpsrv_upld 8080

    and open http://localhost:8080 on your browser
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "utstring.h"
#include <brook.h>

#define USAGE_FMT "Usage: %s <port>\n"
#define UPLOADS_DIR "uploads"

static void srv_req_cb(void *cls, struct b4r_httpsrv_req *req, struct b4r_httpsrv_res *res, bool *done) {
    struct b4r_httpsrv_req_upld *upld;
    UT_string *saved_ol, *err_ol;
    if (strcmp(b4r_httpsrv_req_method(req), "GET") == 0) {
        //TODO: download file using b4r_httpsrv_res_download() as soon as it get ready.
        b4r_httpsrv_res_send(res, "<html><body><form action='' method='post' enctype='multipart/form-data'><fieldset>"
                "<legend>Choose the files:</legend>File 1: <input type='file' name='file1'/><br>"
                "File 2: <input type='file' name='file2'/><br><input type='submit'/></fieldset></form></body></html>");
        return;
    }
    utstring_new(saved_ol);
    utstring_new(err_ol);
    utstring_printf(saved_ol, "<ol>");
    utstring_printf(err_ol, "<ol style='color: red;'>");
    b4r_httpsrv_req_uplds_first(req, &upld);
    while (upld) {
        if (b4r_httpsrv_req_upld_save(req, upld, true))
            utstring_printf(saved_ol, "<li><a href='?file=%s'>%s</a></li>", b4r_httpsrv_req_upld_orig_name(upld),
                            b4r_httpsrv_req_upld_orig_name(upld));
        else
            utstring_printf(err_ol, "<li><font color='red'>%s</font></li>", b4r_httpsrv_req_upld_err(upld));
        b4r_httpsrv_req_uplds_next(&upld);
    }
    utstring_printf(saved_ol, "</ol>");
    utstring_printf(err_ol, "</ol>");
    b4r_httpsrv_res_send(res, "<html><body>Uploads directory '" UPLOADS_DIR "': %s %s</body></html>",
                        utstring_body(saved_ol), utstring_body(err_ol));
    utstring_free(saved_ol);
    utstring_free(err_ol);
}

static void srv_req_err_cb(void *cls, struct b4r_httpsrv_req *req, struct b4r_httpsrv_res *res, bool *done,
                           const char *fmt, va_list va) {
    char err[B4R_ERR_SIZE];
    if (vsnprintf(err, sizeof(err), fmt, va) != -1)
        b4r_httpsrv_res_send(res, "<html><body><font color='red'>%s</font></body></html>", err);
    else
        b4r_httpsrv_res_send_va(res, fmt, va);
}

int main(int argc, const char **argv) {
    struct b4r_httpsrv_cfg *cfg;
    struct b4r_httpsrv *srv;
    uint16_t port;
    bool success;
    if (argc != 2) {
        fprintf(stderr, USAGE_FMT, argv[0]);
        return 1;
    }
    if (NULL == (cfg = b4r_httpsrv_cfg_new(NULL))) {
        fprintf(stderr, "Failed creating configuration.\n");
        return EXIT_FAILURE;
    }
    b4r_httpsrv_cfg_set(cfg, B4R_HTTPSRV_CFG_UPLDS_DIR, UPLOADS_DIR);
    if (NULL == (srv = b4r_httpsrv_new(cfg, &srv_req_cb, NULL, &srv_req_err_cb, NULL, NULL, NULL))) {
        b4r_httpsrv_cfg_free(cfg);
        fprintf(stderr, "Failed creating server.\n");
        return EXIT_FAILURE;
    }
#ifdef WIN32
    mkdir(UPLOADS_DIR);
#else
    mkdir(UPLOADS_DIR, ALLPERMS);
#endif
    port = (uint16_t) atoi(argv[1]);
    b4r_httpsrv_setopt(srv, B4R_HTTPSRV_OPT_PORT, port);
    b4r_httpsrv_setopt(srv, B4R_HTTPSRV_OPT_FORCED_SHUTDOWN, true);
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