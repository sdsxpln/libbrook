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

#ifndef BF_HTTPSRV_STRS_H
#define BF_HTTPSRV_STRS_H

#include <b4r_macros.h>

#define S_BF_NOT_IMPLEMENTED _("Not implemented: %s.\n")
#define S_BF_MAX_ALLOWED_BODY _("Maximum allowed size for body: %s.\n")
#define S_BF_MAX_ALLOWED_PAYLD _("Maximum allowed size for payload: %s.\n")
#define S_BF_MAX_ALLOWED_UPLD _("Maximum allowed size for upload: %s.\n")
#define S_BF_HEADERS_FAILED _("Failed to process haders.\n")
#define S_BF_COOKIES_FAILED _("Failed to process cookies.\n")
#define S_BF_PARAMS_FAILED _("Failed to process parameters.\n")
#define S_BF_FIELD_FAILED _("Failed to process field: %s.\n")
#define S_BF_UPLD_FAILED _("Failed to process upload: %s.\n")
#define S_BF_UPLD_FILE_FAILED _("Failed to %s file '%s': %s")
#define S_BF_UPLD_FILE_FAILED_PREPARE _("prepare")
#define S_BF_UPLD_FILE_FAILED_SAVE _("save")
#define S_BF_UPLD_FILE_FAILED_WRITE _("write")
#define S_BF_SHUTDOWN_FAILED _("Failed to shutdown.\n")
#define S_BF_QUIESCE_FAILED _("Server quiesce failed.\n")

#endif