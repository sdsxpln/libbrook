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

#ifndef BF_HTTPSRV_H
#define BF_HTTPSRV_H

#include <microhttpd.h>
#include "bf_httpsrv_cfg.h"
#include "b4r_httpsrv_auth.h"

struct bf_httpsrv {
    struct bf_httpsrv_cfg *cfg;
    struct _b4r_httpsrv_auth *auth;
    struct MHD_Daemon *mhd;
#ifndef DISABLE_CON_CB
    bf_httpsrv_con_cb con_cb;
    void *con_cls;
#endif
    bf_httpsrv_req_init_cb req_init_cb;
    void *req_init_cls;
    bf_httpsrv_req_fini_cb req_fini_cb;
    void *req_fini_cls;
    bf_httpsrv_req_prep_cb req_prep_cb;
    void *req_prep_cls;
    bf_httpsrv_req_cb req_cb;
    void *req_cls;
    bf_httpsrv_req_upld_data_cb req_upld_data_cb;
    void *req_upld_data_cls;
    bf_httpsrv_req_upld_file_prepare_cb req_upld_file_prepare_cb;
    void *req_upld_file_prepare_cls;
    bf_httpsrv_req_upld_file_save_cb req_upld_file_save_cb;
    void *req_upld_file_save_cls;
    bf_httpsrv_req_upld_file_write_cb req_upld_file_write_cb;
    void *req_upld_file_write_cls;
    bf_httpsrv_req_err_cb req_err_cb;
    void *req_err_cls;
    bf_httpsrv_err_cb err_cb;
    void *err_cls;
    bool req_allowed_post;
    uint16_t port;
    uint8_t shutdown_attempts;
    bool forced_shutdown;
    bool listening;
    bool enabled_log;
};

#endif