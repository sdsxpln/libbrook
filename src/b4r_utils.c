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

#include "config.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "md5.h"
#ifdef WIN32
#include <wchar.h>
#include <rpc.h>
#else
#include <uuid/uuid.h>
#endif
#include "b4r_macros.h"
#include <brook.h>

#ifndef HAVE_STPCPY

char *stpcpy(char *dest, const char *src) {
    char *d;
    const char *s;
    if (!dest || !src)
        return NULL;
    d = dest;
    s = src;
    do
        *d++ = *s;
    while (*s++ != '\0');
    return d - 1;
}

#endif

#ifdef WIN32

wchar_t *stow(const char *str) {
    wchar_t *res = NULL;
    int len;
    if (str) {
        len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, NULL, 0);
        if (len > 0) {
            res = malloc(len * sizeof(wchar_t));
            if (res) {
                if (MultiByteToWideChar(CP_UTF8, 0, str, -1, res, len) == 0) {
                    free(res);
                    return NULL;
                }
            }
        }
    }
    return res;
}

int rename(const char *old_str, const char *new_str) {
    int ret = 0;
    wchar_t *o, *n;
    if ((o = stow(old_str))) {
        if ((n = stow(new_str))) {
            ret = _wrename(o, n);
            free(n);
        }
        free(o);
    }
    return ret;
}

#endif

/* memory */

void *b4r_alloc(size_t size) {
    return calloc(1, size);
}

void b4r_free(void *ptr) {
    free(ptr);
}

/* string */

char *b4r_dup(const char *str) {
    return str ? strdup(str) : strdup("");
}

bool b4r_is_empty(const char *str) {
    unsigned char *tmp;
    if (!str || *str == 0)
        return true;
    for (tmp = (unsigned char *) str; *tmp; tmp++)
        if (!isspace(*tmp))
            return false;
    return true;
}

size_t b4r_index(const char *str, const char *substr) {
    const char *found;
    if (!str || *str == 0 || !substr || *substr == 0)
        return (size_t) -1;
    if (!(found = strstr(str, substr)))
        return (size_t) -1;
    return found - str;
}

char *b4r_lower(char *str) {
    unsigned char *tmp;
    if (!str)
        return NULL;
    tmp = (unsigned char *) str;
    while (*tmp) {
        if (isupper(*tmp))
            *tmp = (unsigned char) tolower(*tmp);
        tmp++;
    }
    return str;
}

char *b4r_upper(char *str) {
    unsigned char *tmp;
    if (!str)
        return NULL;
    tmp = (unsigned char *) str;
    while (*tmp) {
        if (islower(*tmp))
            *tmp = (unsigned char) toupper(*tmp);
        tmp++;
    }
    return str;
}

char *b4r_ltrim(char *str) {
    unsigned char *tmp;
    if (!str)
        return NULL;
    for (tmp = (unsigned char *) str; *tmp && isspace(*tmp); tmp++);
    memmove(str, tmp, strlen((char *) tmp) + 1);
    return str;
}

char *b4r_rtrim(char *str) {
    size_t len;
    if (!str)
        return NULL;
    len = strlen(str);
    while (len--)
        if (isspace((unsigned char) str[len]))
            str[len] = '\0';
        else
            break;
    return str;
}

char *b4r_trim(char *str) {
    return B4R_TRIM(str);
}

char *b4r_cat(const char *str1, const char *str2) {
    char *ret;
    if (!str1 || !str2)
        return NULL;
    if (!(ret = malloc(strlen(str1) + strlen(str2) + sizeof(char))))
        return NULL;
    strcpy(ret, str1);
    strcat(ret, str2);
    return ret;
}

char *b4r_concat(const char *str1, ...) {
    va_list va;
    const char *arg;
    char *ret, *ptr;
    size_t len;
    if (!str1)
        return NULL;
    len = strlen(str1) + sizeof(char);
    va_start(va, str1);
    while ((arg = va_arg(va, const char *)))
        len += strlen(arg);
    va_end(va);
    if (!(ret = malloc(len)))
        return NULL;
    ptr = ret;
    ptr = stpcpy(ptr, str1);
    va_start(va, str1);
    while ((arg = va_arg(va, const char *)))
        ptr = stpcpy(ptr, arg);
    va_end(va);
    return ret;
}

bool b4r_has_suffix(const char *str, const char *suf) {
    size_t str_len, suffix_len;
    if (!str || !suf)
        return false;
    str_len = strlen(str);
    suffix_len = strlen(suf);
    if (str_len < suffix_len)
        return false;
    return strcmp(str + str_len - suffix_len, suf) == 0;
}

bool b4r_has_prefix(const char *str, const char *pre) {
    return (str && pre) && (strncmp(str, pre, strlen(pre)) == 0);
}

size_t b4r_str_to_size(const char *str) {
    return str ? (size_t) strtoumax(str, NULL, 10) : 0;
}

/* TODO: MinGW/Android support. */
char *b4r_asprintf_va(const char *fmt, va_list va) {
    char *res;
    if (vasprintf(&res, fmt, va) != -1)
        return res;
    else
        return NULL;
}

char *b4r_asprintf(const char *fmt, ...) {
    va_list va;
    char *res;
    va_start(va, fmt);
    res = b4r_asprintf_va(fmt, va);
    va_end(va);
    return res;
}

bool b4r_is_post(const char *method) {
    const char **s;
    if (!method)
        return false;
    const char *POST_METHODS[] = {"POST", "PUT", "DELETE", "OPTIONS", NULL};
    for (s = POST_METHODS; *s; s++)
        if (strcmp(method, *s) == 0)
            return true;
    return false;
}

/* file */

char *b4r_add_path_suffix(const char *path) {
    if (b4r_is_empty(path) || b4r_has_suffix(path, DIR_SEP))
        return strdup(path);
    else
        return b4r_cat(path, DIR_SEP);
}

char *b4r_add_path_prefix(const char *path) {
    if (b4r_is_empty(path) || b4r_has_prefix(path, DIR_SEP))
        return strdup(path);
    else
        return b4r_cat(DIR_SEP, path);
}

const char *b4r_tmp_dir() {
    char *buf;
    size_t len;
#define _B4R_ENV_VAR(name) do { \
    buf = getenv(name); \
    if (buf) \
        goto done; \
} while (0)
    _B4R_ENV_VAR("TMPDIR");
    _B4R_ENV_VAR("TMP");
    _B4R_ENV_VAR("TEMP");
    _B4R_ENV_VAR("TEMPDIR");
#undef _B4R_ENV_VAR
    goto unknown;
done:
    len = strlen(buf);
    if (len == 0)
        goto unknown;
#define _B4R_IS_SEP(c) c == '\\' || c == '/' ? true : false
    if (_B4R_IS_SEP(buf[len - 1]))
#undef _B4R_IS_SEP
        buf[len - 1] = '\0';
    return buf;
unknown:
#if defined(__ANDROID__)
    return "/data/local/tmp";
#else
#ifdef _WIN32
    return "tmp";
#else
    return "/tmp";
#endif
#endif
}

char *b4r_fmt_size(uint64_t size) {
    double displayed_size;
#define _B4R_KB_FACTOR (uint64_t) 1024
#define _B4R_MB_FACTOR (_B4R_KB_FACTOR * _B4R_KB_FACTOR)
#define _B4R_GB_FACTOR (_B4R_MB_FACTOR * _B4R_KB_FACTOR)
#define _B4R_TB_FACTOR (_B4R_GB_FACTOR * _B4R_KB_FACTOR)
#define _B4R_PB_FACTOR (_B4R_TB_FACTOR * _B4R_KB_FACTOR)
#define _B4R_EB_FACTOR (_B4R_PB_FACTOR * _B4R_KB_FACTOR)
    if (size < _B4R_KB_FACTOR) {
        if (size > 1)
            return b4r_asprintf(_("%" PRIuS " bytes"), size);
        else
            return b4r_asprintf(_("%" PRIuS " byte"), size);
    } else if (size < _B4R_MB_FACTOR) {
        displayed_size = size / _B4R_KB_FACTOR;
        return b4r_asprintf(_("%.1f KB"), displayed_size);
    } else if (size < _B4R_GB_FACTOR) {
        displayed_size = size / _B4R_MB_FACTOR;
        return b4r_asprintf(_("%.1f MB"), displayed_size);
    } else if (size < _B4R_TB_FACTOR) {
        displayed_size = size / _B4R_GB_FACTOR;
        return b4r_asprintf(_("%.1f GB"), displayed_size);
    } else if (size < _B4R_PB_FACTOR) {
        displayed_size = size / _B4R_TB_FACTOR;
        return b4r_asprintf(_("%.1f TB"), displayed_size);
    } else if (size < _B4R_EB_FACTOR) {
        displayed_size = size / _B4R_PB_FACTOR;
        return b4r_asprintf(_("%.1f PB"), displayed_size);
    } else {
        displayed_size = size / _B4R_EB_FACTOR;
        return b4r_asprintf(_("%.1f EB"), displayed_size);
    }
#undef _B4R_KB_FACTOR
#undef _B4R_MB_FACTOR
#undef _B4R_GB_FACTOR
#undef _B4R_TB_FACTOR
#undef _B4R_PB_FACTOR
#undef _B4R_EB_FACTOR
}

/* hash */

/* TODO: Android support. (JNI purely?) */
bool b4r_uuid(char *uuid) {
    if (!uuid)
        return NULL;
#ifdef WIN32
    UUID buf;
    unsigned char *hash;
    int i;
    if (!(UuidCreateNil(&buf) == RPC_S_OK && UuidCreate(&buf) == RPC_S_OK && UuidToStringA(&buf, &hash) == RPC_S_OK))
        return false;
    for (i = 0; i < B4R_UUID_STR_LEN; i++)
        uuid[i] = (char) tolower(hash[i]);
    RpcStringFree(&hash);
    uuid[B4R_UUID_STR_LEN] = '\0';
    return true;
#else
    uuid_t buf = {};
    if (uuid_generate_time_safe(buf) != 0)
        return false;
    uuid_unparse_lower(buf, uuid);
    return true;
#endif
}

bool b4r_md5(const void *buf, unsigned long size, char *hash) {
    MD5_CTX ctx;
    unsigned char ret[B4R_MD5_BUF_SIZE];
    if (buf || size > 0 || hash) {
        MD5_Init(&ctx);
        MD5_Update(&ctx, buf, size);
        MD5_Final(ret, &ctx);
        return memcpy(hash, ret, sizeof(ret)) != NULL;
    }
    return false;
}

bool b4r_md5_hex(const void *buf, char *hash) {
#define _B4R_UI8H(i, h) do { \
    int8_t l; \
    for (l = 1; l >= 0; i >>= 4, l--) \
        (h)[l] = "0123456789abcdef"[i & 0xf]; \
} while (0)
    int8_t i;
    if (!buf || !hash)
        return false;
    for (i = 0; i < B4R_MD5_BUF_SIZE; i++)
        _B4R_UI8H(((uint8_t *) buf)[i], hash + i * 2);
#undef _B4R_UI8H
    hash[B4R_MD5_SIZE] = '\0';
    return true;
}