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

#ifndef B4R_HTTPSRV_REQ_UPLDS_H
#define B4R_HTTPSRV_REQ_UPLDS_H

#include <stdio.h>
#include <microhttpd.h>
#include "b4r_httpsrv_cfg.h"

struct b4r_httpsrv_req_upld {
    struct b4r_httpsrv_req_upld *next;
    void *stream;
    uint64_t size;
    char *dir;
    char *name;
    char *orig_name;
    char *dest_name;
    char *field;
    char *mime;
    char *encoding;
    bool failed;
    char err[B4R_ERR_SIZE];
};

bool _b4r_httpsrv_req_uplds_new(struct b4r_httpsrv_req_upld **uplds, struct b4r_httpsrv_req_upld **upld,
                               b4r_uuid_func uuid_func, const char *uplds_dir, const char *name, const char *field,
                               const char *mime, const char *encoding);

void _b4r_httpsrv_req_uplds_free(struct b4r_httpsrv_req_upld *upld);

void _b4r_httpsrv_req_upds_cleanup(struct b4r_httpsrv_req_upld *uplds);

void *_b4r_httpsrv_req_upld_file_prepare_cb(void *cls, struct b4r_httpsrv_req_upld *upld);

bool _b4r_httpsrv_req_upld_file_save_cb(void *cls, struct b4r_httpsrv_req_upld *upld, bool overwritten);

bool _b4r_httpsrv_req_upld_file_write_cb(void *cls, struct b4r_httpsrv_req_upld *upld,
                                         const char *buf, size_t size);

bool _b4r_httpsrv_req_uplds_process(struct b4r_httpsrv_req *req, struct MHD_Connection *con, void **con_cls,
                                    const char *upld_data, size_t *upld_data_size, int *ret);

#endif