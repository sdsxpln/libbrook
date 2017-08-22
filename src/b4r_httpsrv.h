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

#ifndef B4R_HTTPSRV_H
#define B4R_HTTPSRV_H

#include <microhttpd.h>
#include "b4r_httpsrv_cfg.h"
#include "b4r_httpsrv_auth.h"

struct b4r_httpsrv {
    struct b4r_httpsrv_cfg *cfg;
    struct _b4r_httpsrv_auth *auth;
    struct MHD_Daemon *mhd;
#ifndef DISABLE_CON_CB
    b4r_httpsrv_con_cb con_cb;
    void *con_cls;
#endif
    b4r_httpsrv_req_init_cb req_init_cb;
    void *req_init_cls;
    b4r_httpsrv_req_fini_cb req_fini_cb;
    void *req_fini_cls;
    b4r_httpsrv_req_prep_cb req_prep_cb;
    void *req_prep_cls;
    b4r_httpsrv_req_cb req_cb;
    void *req_cls;
    b4r_httpsrv_req_upld_data_cb req_upld_data_cb;
    void *req_upld_data_cls;
    b4r_httpsrv_req_upld_file_prepare_cb req_upld_file_prepare_cb;
    void *req_upld_file_prepare_cls;
    b4r_httpsrv_req_upld_file_save_cb req_upld_file_save_cb;
    void *req_upld_file_save_cls;
    b4r_httpsrv_req_upld_file_write_cb req_upld_file_write_cb;
    void *req_upld_file_write_cls;
    b4r_httpsrv_req_err_cb req_err_cb;
    void *req_err_cls;
    b4r_httpsrv_err_cb err_cb;
    void *err_cls;
    bool req_allowed_post;
    uint16_t port;
    uint8_t shutdown_attempts;
    bool forced_shutdown;
    bool listening;
    bool enabled_log;
};

#endif