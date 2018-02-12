/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\
 *
 *   –– a small library which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2018 Silvio Clecio <silvioprog@gmail.com>
 *
 * This file is part of Brook library.
 *
 * Brook library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brook library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Brook library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @mainpage
 * @li @ref brook_api
 */

/**
 * @defgroup brook_api API reference
 * @{
 */

#ifndef BROOK_H
#define BROOK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define BK_VERSION_MAJOR 0
#define BK_VERSION_MINOR 0
#define BK_VERSION_PATCH 1
#define BK_VERSION_HEX ((BK_VERSION_MAJOR << 16) | (BK_VERSION_MINOR <<  8) | (BK_VERSION_PATCH))

/* Utilities */

/**
 * Returns the library version number.
 * @return Library version packed into a single integer.
 */
extern unsigned int bk_version(void);

/**
 * Returns the library version number as string.
 * @return Library version packed into a static string.
 */
extern const char *bk_version_str(void);

/**
 * Allocates a new memory space and zero-initialize it.
 * @param[in] size Memory size to be allocated.
 * @return Pointer of the allocated and zero-initialized memory.
 * @note If @p size is  `0`, then #bk_new() returns either `NULL`, or a unique pointer value that can later be
 * successfully passed to #bk_free().
 */
extern void *bk_new(size_t size);

/**
 * Frees a memory space previous allocated by #bk_new().
 * @param[in] ptr Pointer of the memory to be freed.
 */
extern void bk_free(void *ptr);

/* String-buffer */

/**
 * Handle for the string-buffer. It is used to represent a HTML body, POST payload and more.
 */
struct bk_strbuf;

/**
 * Creates a new zero-initialized string-buffer instance.
 * @return String-buffer zero-initialized instance.
 * @note If #bk_strbuf_new fails (e.g: no memory space), a `NULL` is returned.
 */
extern struct bk_strbuf *bk_strbuf_new(void);

/**
 * Frees a string-buffer instance previous allocated by #bk_strbuf_new().
 * @param[in] sb Pointer of the string-buffer to be freed.
 */
extern void bk_strbuf_free(struct bk_strbuf *sb);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BROOK_H */