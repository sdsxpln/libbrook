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

#ifndef B4R_HTTPSRV_STRS_H
#define B4R_HTTPSRV_STRS_H

#include <b4r_macros.h>

#define S_B4R_NOT_IMPLEMENTED _("Not implemented: %s.\n")
#define S_B4R_MAX_ALLOWED_BODY _("Maximum allowed size for body: %s.\n")
#define S_B4R_MAX_ALLOWED_PAYLD _("Maximum allowed size for payload: %s.\n")
#define S_B4R_MAX_ALLOWED_UPLD _("Maximum allowed size for upload: %s.\n")
#define S_B4R_HEADERS_FAILED _("Failed to process haders.\n")
#define S_B4R_COOKIES_FAILED _("Failed to process cookies.\n")
#define S_B4R_PARAMS_FAILED _("Failed to process parameters.\n")
#define S_B4R_FIELD_FAILED _("Failed to process field: %s.\n")
#define S_B4R_UPLD_FAILED _("Failed to process upload: %s.\n")
#define S_B4R_UPLD_FILE_FAILED _("Failed to %s file '%s': %s")
#define S_B4R_UPLD_FILE_FAILED_PREPARE _("prepare")
#define S_B4R_UPLD_FILE_FAILED_SAVE _("save")
#define S_B4R_UPLD_FILE_FAILED_WRITE _("write")
#define S_B4R_SHUTDOWN_FAILED _("Failed to shutdown.\n")
#define S_B4R_QUIESCE_FAILED _("Server quiesce failed.\n")

#endif