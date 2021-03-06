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

bool _b4r_httpsrv_req_uplds_try_new(struct b4r_httpsrv_req_upld **uplds, struct b4r_httpsrv_req_upld **upld,
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