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

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "b4r_macros.h"
#ifdef WIN
#include <unistd.h>
#include <in6addr.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include "b4r_httpsrv_strs.h"
#include "b4r_httpsrv_req_uplds.h"
#include "b4r_httpsrv_req.h"
#include "b4r_httpsrv_res.h"
#include "b4r_httpsrv.h"

static void _b4r_httpsrv_err_va_cb(void *cls, const char *fmt, va_list va) {
    struct b4r_httpsrv *srv = cls;
    char err[B4R_ERR_SIZE];
    if (srv->err_cb) {
        vsnprintf(err, sizeof(err), fmt, va);
        srv->err_cb(srv->err_cls, err);
    }
    else if (srv->enabled_log)
        vfprintf(stdout, fmt, va);
}

static void _b4r_httpsrv_err_cb(void *cls, const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    _b4r_httpsrv_err_va_cb(cls, fmt, va);
    va_end(va);
}

struct b4r_httpsrv *b4r_httpsrv_new(struct b4r_httpsrv_cfg *cfg, b4r_httpsrv_req_cb req_cb, void *req_cls,
                                    b4r_httpsrv_req_err_cb req_err_cb, void *req_err_cls,
                                    b4r_httpsrv_err_cb err_cb, void *err_cls) {
    struct b4r_httpsrv *srv;
    if (!cfg || !req_cb)
        return NULL;
    _B4R_NEW(&srv);
    srv->auth = _b4r_httpsrv_auth_new();
    srv->cfg = cfg;
    srv->req_cb = req_cb;
    srv->req_cls = req_cls;
    srv->req_err_cb = req_err_cb;
    srv->req_err_cls = req_err_cls;
    srv->req_allowed_post = true;
    srv->err_cb = err_cb;
    srv->err_cls = err_cls;
    srv->port = cfg->port;
    srv->forced_shutdown = true;
    return srv;
}

void b4r_httpsrv_free(struct b4r_httpsrv *srv) {
    if (srv) {
        _b4r_httpsrv_auth_free(srv->auth);
        _B4R_FREE(srv);
    }
}

#ifndef DISABLE_CON_CB

static void _b4r_httpsrv_con_cb(void *cls, struct MHD_Connection *con, void **socket_ctx,
                                enum MHD_ConnectionNotificationCode toe) {
    struct b4r_httpsrv *srv;
    const union MHD_ConnectionInfo *info;
    char ipv4[INET_ADDRSTRLEN];
    char ipv6[INET6_ADDRSTRLEN];
    switch (toe) {
        case MHD_CONNECTION_NOTIFY_STARTED:
            info = MHD_get_connection_info(con, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
#define _ID_FMT "%x:%s"
#ifdef WIN32
        switch (info->client_addr->sa_family) {
                case AF_INET:
                    getnameinfo(info->client_addr, sizeof(struct sockaddr), ipv4, INET_ADDRSTRLEN, NULL, 0,
                                NI_NUMERICHOST);
                    *socket_ctx = b4r_asprintf(_ID_FMT, info->connect_fd, ipv4);
                    break;
                case AF_INET6:
                    getnameinfo(info->client_addr, sizeof(struct sockaddr_in6), ipv6, INET6_ADDRSTRLEN, NULL, 0,
                                NI_NUMERICHOST);
                    *socket_ctx = b4r_asprintf(_ID_FMT, info->connect_fd, ipv6);
                    break;
                default:
                    break;
            }
#else
            switch (info->client_addr->sa_family) {
                case AF_INET:
                    inet_ntop(AF_INET, &(((struct sockaddr_in *) info->client_addr)->sin_addr), ipv4, INET_ADDRSTRLEN);
                    *socket_ctx = b4r_asprintf(_ID_FMT, info->connect_fd, ipv4);
                    break;
                case AF_INET6:
                    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *) info->client_addr)->sin6_addr),
                              ipv6, INET6_ADDRSTRLEN);
                    *socket_ctx = b4r_asprintf(_ID_FMT, info->connect_fd, ipv6);
                    break;
                default:
                    break;
            }
#endif
#undef _ID_FMT
            if ((srv = cls) && srv->con_cb)
                srv->con_cb(srv->con_cls, *socket_ctx, false);
            break;
        case MHD_CONNECTION_NOTIFY_CLOSED:
            if ((srv = cls) && srv->con_cb)
                srv->con_cb(srv->con_cls, *socket_ctx, true);
            _B4R_FREE(*socket_ctx);
            break;
    }
}

#endif

bool b4r_httpsrv_listen(struct b4r_httpsrv *srv) {
    if (!srv)
        return false;
    if (!srv->listening) {
        if (!srv->req_upld_file_prepare_cb && !srv->req_upld_file_save_cb && !srv->req_upld_file_write_cb) {
            srv->req_upld_file_prepare_cb = _b4r_httpsrv_req_upld_file_prepare_cb;
            srv->req_upld_file_save_cb = _b4r_httpsrv_req_upld_file_save_cb;
            srv->req_upld_file_write_cb = _b4r_httpsrv_req_upld_file_write_cb;
        }
        srv->listening = (srv->mhd = MHD_start_daemon(
                MHD_USE_SELECT_INTERNALLY | MHD_USE_ITC | MHD_USE_ERROR_LOG, srv->port, NULL, NULL,
                &_b4r_httpsrv_req_cb, srv,
#ifndef DISABLE_CON_CB
                MHD_OPTION_NOTIFY_CONNECTION, &_b4r_httpsrv_con_cb, srv,
#endif
                MHD_OPTION_URI_LOG_CALLBACK, &_b4r_httpsrv_req_init_cb, srv,
                MHD_OPTION_NOTIFY_COMPLETED, &_b4r_httpsrv_req_fini_cb, srv,
                MHD_OPTION_EXTERNAL_LOGGER, &_b4r_httpsrv_err_va_cb, srv,
                MHD_OPTION_END)) != NULL;
    }
    return srv->listening;
}

bool b4r_httpsrv_listening(struct b4r_httpsrv *srv) {
    return srv && srv->listening;
}

bool b4r_httpsrv_shutdown(struct b4r_httpsrv *srv) {
    MHD_socket fd;
    uint8_t shutdown_attempts;
    if (srv && srv->listening) {
        fd = MHD_quiesce_daemon(srv->mhd);
        if (fd != MHD_INVALID_SOCKET) {
            if (srv->forced_shutdown)
#ifdef WINNT
                shutdown(fd, SD_BOTH);
#else
                shutdown(fd, SHUT_RDWR);
#endif
#ifdef WINNT
            closesocket(fd);
#else
            close(fd);
#endif
            if (!srv->forced_shutdown && srv->shutdown_attempts > 0) {
                shutdown_attempts = 1;
                while (MHD_get_daemon_info(srv->mhd, MHD_DAEMON_INFO_CURRENT_CONNECTIONS)->num_connections > 0) {
                    if (shutdown_attempts >= srv->shutdown_attempts) {
                        _b4r_httpsrv_err_cb(srv, S_B4R_SHUTDOWN_FAILED);
                        break;
                    }
                    shutdown_attempts++;
                    sleep(1);
                }
            }
        } else
            _b4r_httpsrv_err_cb(srv, S_B4R_QUIESCE_FAILED);
        MHD_stop_daemon(srv->mhd);
        srv->listening = false;
        return true;
    }
    return false;
}

bool b4r_httpsrv_setopt(struct b4r_httpsrv *srv, enum B4R_HTTPSRV_OPT opt, ...) {
    va_list va;
    if (b4r_httpsrv_listening(srv))
        return false;
    va_start(va, opt);
    switch (opt) {
        case B4R_HTTPSRV_OPT_CON_CB:
#ifndef DISABLE_CON_CB
            srv->con_cb = va_arg(va, b4r_httpsrv_con_cb);
            srv->con_cls = va_arg(va, void *);
            return srv->con_cb != NULL;
#else
            _B4R_LOG(srv->enabled_log, S_B4R_NOT_IMPLEMENTED, _B4R_STR(B4R_HTTPSRV_OPT_CON_CB));
            return false;
#endif
        case B4R_HTTPSRV_OPT_REQ_INIT_CB:
            srv->req_init_cb = va_arg(va, b4r_httpsrv_req_init_cb);
            srv->req_init_cls = va_arg(va, void *);
            return srv->req_init_cb != NULL;
        case B4R_HTTPSRV_OPT_REQ_FINI_CB:
            srv->req_fini_cb = va_arg(va, b4r_httpsrv_req_fini_cb);
            srv->req_fini_cls = va_arg(va, void *);
            return srv->req_fini_cb != NULL;
        case B4R_HTTPSRV_OPT_REQ_PREP_CB:
            srv->req_prep_cb = va_arg(va, b4r_httpsrv_req_prep_cb);
            srv->req_prep_cls = va_arg(va, void *);
            return srv->req_prep_cb != NULL;
        case B4R_HTTPSRV_OPT_REQ_UPLD_DATA_CB:
            srv->req_upld_data_cb = va_arg(va, b4r_httpsrv_req_upld_data_cb);
            srv->req_upld_data_cls = va_arg(va, void *);
            return srv->req_upld_data_cb != NULL;
        case B4R_HTTPSRV_OPT_REQ_UPLD_FILE_CBS:
            srv->req_upld_file_prepare_cb = va_arg(va, b4r_httpsrv_req_upld_file_prepare_cb);
            srv->req_upld_file_prepare_cls = va_arg(va, void *);
            srv->req_upld_file_save_cb = va_arg(va, b4r_httpsrv_req_upld_file_save_cb);
            srv->req_upld_file_save_cls = va_arg(va, void *);
            srv->req_upld_file_write_cb = va_arg(va, b4r_httpsrv_req_upld_file_write_cb);
            srv->req_upld_file_write_cls = va_arg(va, void *);
            return srv->req_upld_file_prepare_cb && srv->req_upld_file_save_cb && srv->req_upld_file_write_cb;
        case B4R_HTTPSRV_OPT_REQ_ERR_CB:
            srv->req_err_cb = va_arg(va, b4r_httpsrv_req_err_cb);
            srv->req_err_cls = va_arg(va, void *);
            return srv->req_err_cb != NULL;
        case B4R_HTTPSRV_OPT_REQ_ALLOWED_POST:
            srv->req_allowed_post = (bool) va_arg(va, int);
            return true;
        case B4R_HTTPSRV_OPT_ERR_CB:
            srv->err_cb = va_arg(va, b4r_httpsrv_err_cb);
            srv->err_cls = va_arg(va, void *);
            return srv->req_err_cb != NULL;
        case B4R_HTTPSRV_OPT_PORT:
            srv->port = (uint16_t) va_arg(va, unsigned int);
            return srv->port > 0;
        case B4R_HTTPSRV_OPT_SHUTDOWN_ATTEMPTS:
            srv->shutdown_attempts = (uint8_t) va_arg(va, unsigned int);
            return srv->shutdown_attempts > 0;
        case B4R_HTTPSRV_OPT_FORCED_SHUTDOWN:
            srv->forced_shutdown = (bool) va_arg(va, int);
            return true;
        case B4R_HTTPSRV_OPT_AUTH:
            srv->auth->type = va_arg(va, enum B4R_HTTPSRV_AUTH_TYPE);
            srv->auth->active = (bool) va_arg(va, int);
            srv->auth->realm = b4r_dup(va_arg(va, const char *));
            srv->auth->err = b4r_dup(va_arg(va, const char *));
            srv->auth->user = b4r_dup(va_arg(va, const char *));
            srv->auth->pwd = b4r_dup(va_arg(va, const char *));
            switch (srv->auth->type) {
                case B4R_HTTPSRV_AUTH_TYPE_BASIC:
                    srv->auth->handle = _b4r_httpsrv_auth_basic_handle;
                    break;
                case B4R_HTTPSRV_AUTH_TYPE_DIGEST:
                    srv->auth->handle = _b4r_httpsrv_auth_digest_handle;
                    break;
            }
            return !b4r_is_empty(srv->auth->user) && srv->auth->pwd;
        case B4R_HTTPSRV_OPT_ENABLED_LOG:
            srv->enabled_log = (bool) va_arg(va, int);
            return true;
        default:
            return false;
    }
    va_end(va);
}

unsigned int b4r_httpsrv_active_conns(struct b4r_httpsrv *srv) {
    return srv ? MHD_get_daemon_info(srv->mhd, MHD_DAEMON_INFO_CURRENT_CONNECTIONS)->num_connections : 0;
}