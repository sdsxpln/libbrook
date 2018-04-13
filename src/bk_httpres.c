#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "microhttpd.h"
#include "bk_macros.h"
#if defined(__MINGW32__) || defined(__ANDROID__)
#include "bk_utils.h" /* basename() */
#endif
#include "brook.h"
#include "bk_strmap.h"
#include "bk_httpres.h"

static int bk__httpheaders_iter(void *cls, struct bk_strmap *header) {
    if (MHD_add_response_header(cls, header->name, header->val) != MHD_YES)
        return -ENOMEM;
    return 0;
}

static ssize_t bk__httpfileread_cb(void *cls, __BK_UNUSED uint64_t offset, char *buf, size_t size) {
    return
#ifdef __ANDROID__
        (ssize_t)
#endif
            fread(buf, sizeof(char), size, cls);
}

static void bk__httpfilefree_cb(void *cls) {
    fclose(cls);
}

void bk__httpres_init(struct bk_httpres *res, struct MHD_Connection *con) {
    memset(res, 0, sizeof(struct bk_httpres));
    res->con = con;
    res->status = 500;
}

int bk__httpres_done(struct bk_httpres *res) {
    if (res->headers && bk_strmap_iter(res->headers, bk__httpheaders_iter, res->handle) != 0) {
        bk_strmap_cleanup(&res->headers);
        MHD_destroy_response(res->handle);
        oom();
    }
    bk_strmap_cleanup(&res->headers);
    res->ret = MHD_queue_response(res->con, res->status, res->handle);
    MHD_destroy_response(res->handle);
    return res->ret;
}

struct bk_strmap **bk_httpres_headers(struct bk_httpres *res) {
    return res ? &res->headers : NULL;
}

int bk_httpres_send(struct bk_httpres *res, const char *val, const char *content_type, unsigned int status) {
    return bk_httpres_sendbinary(res, (void *) val, (val ? strlen(val) : 0), content_type, status);
}

int bk_httpres_sendbinary(struct bk_httpres *res, void *buf, size_t size, const char *content_type,
                          unsigned int status) {
    if (!res || !buf || !content_type || ((status < 100) || (status > 599)))
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(res->handle = MHD_create_response_from_buffer(size, buf, MHD_RESPMEM_MUST_COPY)))
        oom();
    bk_strmap_set(&res->headers, MHD_HTTP_HEADER_CONTENT_TYPE, content_type);
    res->status = status;
    return 0;
}

int bk_httpres_sendstr(struct bk_httpres *res, struct bk_str *str, const char *content_type, unsigned int status) {
    return bk_httpres_sendbinary(res, (void *) bk_str_content(str), bk_str_length(str), content_type, status);
}

int bk_httpres_sendfile(struct bk_httpres *res, size_t block_size, uint64_t max_size, const char *filename,
                        bool rendered, unsigned int status) {
    FILE *file;
    struct stat64 sbuf;
    char *absolute_path;
    const char *cd_type;
    const char *cd_basename;
    char *cd_header;
    size_t fn_size;
    int fd, eno = 0;
    if (!res || (block_size < 1) || ((int64_t) max_size < 0) || !filename || ((status < 100) || (status > 599)))
        return -EINVAL;
    if (res->handle)
        return -EALREADY;
    if (!(absolute_path = realpath(filename, NULL)))
        return -errno;
#ifdef _WIN32
    eno = fopen_s(&file, absolute_path, "rb");
    if (eno)
        goto fail;
#else
    if (!(file = fopen(absolute_path, "rb"))) {
        eno = errno;
        goto fail;
    }
#endif
    if ((fd = fileno(file)) == -1) {
        eno = errno;
        goto fail;
    }
    if (fstat64(fd, &sbuf)) {
        eno = errno;
        goto fail;
    }
    if ((max_size > 0) && ((uint64_t) sbuf.st_size > max_size)) {
        eno = EFBIG;
        goto fail;
    }
    if (S_ISDIR(sbuf.st_mode)) {
        eno = EISDIR;
        goto fail;
    }
    if (!S_ISREG(sbuf.st_mode)) {
        eno = EBADF;
        goto fail;
    }
#define _BK_FNFMT "%s; filename=\"%s\""
    cd_type = rendered ? "inline" : "attachment";
    cd_basename = basename(absolute_path);
    fn_size = snprintf(NULL, 0, _BK_FNFMT, cd_type, cd_basename) + sizeof(char);
    if (!(cd_header = bk_alloc(fn_size))) {
        eno = ENOMEM;
        goto fail;
    }
    snprintf(cd_header, fn_size, _BK_FNFMT, cd_type, cd_basename);
    bk_free(absolute_path);
#undef _BK_FNFMT
    bk_strmap_set(&res->headers, MHD_HTTP_HEADER_CONTENT_DISPOSITION, cd_header);
    bk_free(cd_header);
    if (!(res->handle = MHD_create_response_from_callback((uint64_t) sbuf.st_size, block_size, bk__httpfileread_cb,
                                                          file, bk__httpfilefree_cb))) {
        eno = ENOMEM;
        goto fail;
    }
    res->status = status;
    return 0;
fail:
    bk_free(absolute_path);
    if (file) {
        if (eno == 0)
            eno = fclose(file);
        else
            fclose(file);
    }
    if (eno == ENOMEM)
        oom();
    return -eno;
}

int bk_httpres_sendstream(struct bk_httpres *res, uint64_t size, size_t block_size, bk_httpread_cb write_cb, void *cls,
                          bk_httpfree_cb free_cb, unsigned int status) {
    int eno;
    if (!res || (size < 1) || (block_size < 1) || !write_cb || ((status < 100) || (status > 599))) {
        eno = EINVAL;
        goto failed;
    }
    if (res->handle) {
        eno = EALREADY;
        goto failed;
    }
    if (!(res->handle = MHD_create_response_from_callback(size, block_size, write_cb, cls, free_cb))) {
        eno = ENOMEM;
        goto failed;
    }
    res->status = status;
    return 0;
failed:
    if (free_cb)
        free_cb(cls);
    if (eno == ENOMEM)
        oom();
    return -eno;
}

int bk_httpres_senddata(struct bk_httpres *res, size_t block_size, bk_httpread_cb read_cb, void *cls,
                        bk_httpfree_cb free_cb, unsigned int status) {
    return bk_httpres_sendstream(res, MHD_SIZE_UNKNOWN, block_size, read_cb, cls, free_cb, status);
}
