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

#include <unistd.h>
#include "utlist.h"
#include "bf_httpsrv_strs.h"
#include "bf_httpsrv_req_uplds.h"
#include "bf_httpsrv_req.h"
#include "bf_httpsrv.h"

#define _BF_HTTPSRV_REQ_UPLDS_GET_PROP(el, mb, def) (el) ? el->mb : def

bool _bf_httpsrv_req_uplds_new(struct bf_httpsrv_req_upld **uplds, struct bf_httpsrv_req_upld **upld,
                               bf_uuid_func uuid_func, const char *uplds_dir, const char *name, const char *field,
                               const char *mime, const char *encoding) {
    char uuid[B4R_UUID_STR_LEN + 1];
    _B4R_NEW(upld);
    (*upld)->failed = true;
    if (!uuid_func(uuid))
        return false;
    if (!((*upld)->dir = b4r_add_path_suffix(uplds_dir)))
        return false;
    if (!((*upld)->name = b4r_concat((*upld)->dir, "brook_upload_", uuid, NULL)))
        return false;
    if (!((*upld)->dest_name = b4r_cat((*upld)->dir, name)))
        return false;
    (*upld)->orig_name = strdup(name);
    (*upld)->field = b4r_dup(field);
    (*upld)->mime = b4r_dup(mime);
    (*upld)->encoding = b4r_dup(encoding);
    LL_APPEND(*uplds, *upld);
    return true;
}

void _bf_httpsrv_req_uplds_free(struct bf_httpsrv_req_upld *upld) {
    if (upld->failed)
        unlink(upld->name);
    _B4R_FREE(upld->dir);
    _B4R_FREE(upld->name);
    _B4R_FREE(upld->orig_name);
    _B4R_FREE(upld->dest_name);
    _B4R_FREE(upld->field);
    _B4R_FREE(upld->mime);
    _B4R_FREE(upld->encoding);
    _B4R_FREE(upld);
}

void _bf_httpsrv_req_upds_cleanup(struct bf_httpsrv_req_upld *uplds) {
    struct bf_httpsrv_req_upld *upld, *tmp;
    LL_FOREACH_SAFE(uplds, upld, tmp) {
        LL_DELETE(uplds, upld);
        _bf_httpsrv_req_uplds_free(upld);
    }
}

static void _bf_httpsrv_req_upld_file_fail(struct bf_httpsrv_req_upld *upld, const char *op) {
    snprintf(upld->err, sizeof(upld->err), S_BF_UPLD_FILE_FAILED, op, upld->orig_name, strerror(errno));
}

void *_bf_httpsrv_req_upld_file_prepare_cb(void *cls, struct bf_httpsrv_req_upld *upld) {
    FILE *stream = fopen(upld->name, "wb");
    if (!stream)
        _bf_httpsrv_req_upld_file_fail(upld, S_BF_UPLD_FILE_FAILED_PREPARE);
    return stream;
}

bool _bf_httpsrv_req_upld_file_save_cb(void *cls, struct bf_httpsrv_req_upld *upld, bool overwritten) {
    FILE *stream = upld->stream;
    if (!stream || fclose(stream) != 0 ||
        (overwritten && access(upld->dest_name, F_OK) != -1 && unlink(upld->dest_name) != 0) ||
        rename(upld->name, upld->dest_name) != 0) {
        _bf_httpsrv_req_upld_file_fail(upld, S_BF_UPLD_FILE_FAILED_SAVE);
        return false;
    }
    return true;
}

bool _bf_httpsrv_req_upld_file_write_cb(void *cls, struct bf_httpsrv_req_upld *upld,
                                        const char *buf, size_t size) {
    if (fwrite(buf, sizeof(char), size, upld->stream) != size) {
        _bf_httpsrv_req_upld_file_fail(upld, S_BF_UPLD_FILE_FAILED_WRITE);
        return false;
    }
    return true;
}

static int _bf_httpsrv_req_uplds_iter(void *cls, enum MHD_ValueKind kind, const char *key, const char *filename,
                                      const char *content_type, const char *transfer_encoding, const char *buf,
                                      uint64_t offset, size_t size) {
    struct b4r_httpsrv_req *req;
    char *max_upld_size_str;
    if (kind == MHD_POSTDATA_KIND && size > 0) {
        req = cls;
        if (filename) {
            if (offset == 0) {
                if (!_bf_httpsrv_req_uplds_new(&req->uplds, &req->cur_upld, req->owner->cfg->uuid_func,
                                               req->owner->cfg->uplds_dir, filename, key, content_type,
                                               transfer_encoding)) {
                    _bf_httpsrv_req_err(req, S_BF_UPLD_FAILED, filename);
                    return MHD_NO;
                }
                if (!(req->cur_upld->stream = req->owner->req_upld_file_prepare_cb(
                        req->owner->req_upld_file_prepare_cls, req->cur_upld)))
                    return MHD_YES;
            }
            if (!req->owner->req_upld_file_write_cb(req->owner->req_upld_file_write_cls, req->cur_upld, buf, size))
                return MHD_YES;
            req->cur_upld->size += size;
            if (req->owner->cfg->max_upld_size > 0) {
                req->total_upld_size += size;
                if (req->total_upld_size > req->owner->cfg->max_upld_size) {
                    max_upld_size_str = b4r_fmt_size(req->owner->cfg->max_upld_size);
                    _bf_httpsrv_req_err(req, S_BF_MAX_ALLOWED_UPLD, max_upld_size_str);
                    _B4R_FREE(max_upld_size_str);
                    return MHD_NO;
                }
            }
        } else if (!b4r_hs_add_or_set(&req->fields, key, buf)) {
            _bf_httpsrv_req_err(req, S_BF_FIELD_FAILED, key);
            return MHD_NO;
        }
    }
    return MHD_YES;
}

bool _bf_httpsrv_req_uplds_process(struct b4r_httpsrv_req *req, struct MHD_Connection *con, void **con_cls,
                                   const char *upld_data, size_t *upld_data_size, int *ret) {
    char *max_payld_size_str;
    char err[BF_ERR_SIZE];
    if (*upld_data_size > 0) {
        if (req->uploading) {
            if (req->owner->req_upld_data_cb) {
                req->uploading = req->owner->req_upld_data_cb(req->owner->req_upld_data_cls, req, upld_data,
                                                              *upld_data_size, err);
                if (!req->uploading)
                    _bf_httpsrv_req_err(req, "%s", err);
            } else {
                if (!req->post_proc)
                    req->post_proc = MHD_create_post_processor(con, req->owner->cfg->post_buffer_size,
                                                               &_bf_httpsrv_req_uplds_iter, *con_cls);
                if (req->post_proc)
                    req->uploading = MHD_post_process(req->post_proc, upld_data, *upld_data_size) == MHD_YES;
                else {
                    utstring_bincpy(req->payload, upld_data, *upld_data_size);
                    if (req->owner->cfg->max_payld_size > 0) {
                        if (utstring_len(req->payload) > req->owner->cfg->max_payld_size) {
                            req->uploading = false;
                            max_payld_size_str = b4r_fmt_size(req->owner->cfg->max_payld_size);
                            _bf_httpsrv_req_err(req, S_BF_MAX_ALLOWED_PAYLD, max_payld_size_str);
                            _B4R_FREE(max_payld_size_str);
                        }
                    }
                }
            }
        }
        *upld_data_size = 0;
        *ret = MHD_YES;
        return true;
    }
    return false;
}

bool bf_httpsrv_req_uplds_first(struct b4r_httpsrv_req *req, struct bf_httpsrv_req_upld **upld) {
    if (!req || !upld)
        return false;
    if (req->uplds)
        *upld = req->uplds;
    else
        *upld = NULL;
    return true;
}

bool bf_httpsrv_req_uplds_next(struct bf_httpsrv_req_upld **upld) {
    if (!upld)
        return false;
    if (*upld)
        *upld = (*upld)->next;
    else
        *upld = NULL;
    return true;
}

int bf_httpsrv_req_uplds_count(struct b4r_httpsrv_req *req) {
    struct bf_httpsrv_req_upld *upld;
    int count;
    if (!req)
        return -1;
    if (!req->uplds)
        return 0;
    LL_COUNT(req->uplds, upld, count);
    return count;
}

bool bf_httpsrv_req_upld_save(struct b4r_httpsrv_req *req, struct bf_httpsrv_req_upld *upld, bool overwritten) {
    return req && upld &&
           !(upld->failed = !req->owner->req_upld_file_save_cb(req->owner->req_upld_file_save_cls, upld, overwritten));
}

bool bf_httpsrv_req_upld_save_as(struct b4r_httpsrv_req *req, struct bf_httpsrv_req_upld *upld, const char *name,
                                 bool overwritten) {
    if (!bf_httpsrv_req_upld_save(req, upld, overwritten) || (upld->failed = b4r_is_empty(name)))
        return false;
    if ((upld->failed = (overwritten && access(name, F_OK) != -1 && unlink(name) != 0) ||
                        rename(upld->dest_name, name) != 0)) {
        _bf_httpsrv_req_upld_file_fail(upld, S_BF_UPLD_FILE_FAILED_SAVE);
        return false;
    }
    return true;
}

void *bf_httpsrv_req_upld_stream(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, stream, NULL);
}

uint64_t bf_httpsrv_req_upld_size(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, size, 0);
}

const char *bf_httpsrv_req_upld_name(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, name, NULL);
}

const char *bf_httpsrv_req_upld_orig_name(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, orig_name, NULL);
}

const char *bf_httpsrv_req_upld_dir(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, dir, NULL);
}

const char *bf_httpsrv_req_upld_dest_name(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, dest_name, NULL);
}

const char *bf_httpsrv_req_upld_field(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, field, NULL);
}

const char *bf_httpsrv_req_upld_mime(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, mime, NULL);
}

const char *bf_httpsrv_req_upld_encoding(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, encoding, NULL);
}

const char *bf_httpsrv_req_upld_err(struct bf_httpsrv_req_upld *upld) {
    return _BF_HTTPSRV_REQ_UPLDS_GET_PROP(upld, err, NULL);
}

int bf_httpsrv_req_upld_failf_va(struct bf_httpsrv_req_upld *upld, const char *fmt, va_list va) {
    return upld ? vsnprintf(upld->err, sizeof(upld->err), fmt, va) : -1;
}

int bf_httpsrv_req_upld_failf(struct bf_httpsrv_req_upld *upld, const char *fmt, ...) {
    va_list va;
    int ret;
    if (!upld)
        return -1;
    va_start(va, fmt);
    ret = vsnprintf(upld->err, sizeof(upld->err), fmt, va);
    va_end(va);
    return ret;
}

#undef _BF_HTTPSRV_REQ_UPLDS_GET_PROP