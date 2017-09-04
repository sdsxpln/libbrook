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

#ifndef B4R_HTTPSRV_REQ_H
#define B4R_HTTPSRV_REQ_H

#include <stdbool.h>
#include "utstring.h"
#include <microhttpd.h>
#include "b4r_hs.h"
#include <brook.h>

struct b4r_httpsrv_req {
    struct b4r_httpsrv *owner;
    struct b4r_httpsrv_res *res;
    struct MHD_Connection *con;
    struct MHD_PostProcessor *post_proc;
    struct b4r_hs *headers;
    struct b4r_hs *cookies;
    struct b4r_hs *params;
    struct b4r_hs *fields;
    struct b4r_httpsrv_req_upld *uplds;
    struct b4r_httpsrv_req_upld *cur_upld;
    bool receiving;
    bool uploading;
    bool aborted;
    bool done;
    size_t total_upld_size;
    size_t content_len;
    char *id;
    char *uri;
    const char *content_type;
    const char *version;
    const char *method;
    const char *path;
    UT_string *payload;
};

void *_b4r_httpsrv_req_init_cb(void *cls, const char *uri, struct MHD_Connection *con);

void _b4r_httpsrv_req_fini_cb(void *cls, struct MHD_Connection *con, void **con_cls,
                              enum MHD_RequestTerminationCode toe);

int _b4r_httpsrv_req_cb(void *cls, struct MHD_Connection *con, const char *url, const char *method, const char *version,
                        const char *upld_data, size_t *upld_data_size, void **con_cls);

void _b4r_httpsrv_req_err(struct b4r_httpsrv_req *req, const char *fmt, ...) __format(2, 3);

#endif