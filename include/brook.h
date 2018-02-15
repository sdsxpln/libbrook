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
 * @li @ref bk_api
 */

/** @defgroup bk_api API reference
 * The API reference grouped by feature.
 * */

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

/**
 * @ingroup bk_api
 * @defgroup bk_utils Utilities
 * All utility functions of the library.
 * @{
 */

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
 * @return Pointer of the allocated zero-initialized memory.
 * @retval NULL When size is `0` or no memory space.
 */
extern void *bk_alloc(size_t size);

/**
 * Frees a memory space previous allocated by #bk_alloc().
 * @param[in] ptr Pointer of the memory to be freed.
 */
extern void bk_free(void *ptr);

/** @} */

/**
 * @ingroup bk_api
 * @defgroup bk_str String
 * String structure and its related functions.
 * @{
 */

/**
 * Handle for the string structure used to represent a HTML body, POST payload and more.
 */
struct bk_str;

/**
 * Creates a new string instance zero-initialized.
 * @return String instance zero-initialized.
 * @retval NULL When no memory space.
 */
extern struct bk_str *bk_str_new(void);

/**
 * Frees a string instance previous allocated by #bk_str_new().
 * @param[in] str Pointer of the string instance to be freed.
 */
extern void bk_str_free(struct bk_str *str);

/**
 * Writes a value into string handle @p str. All values previously written are kept.
 * @param[in] str String handle.
 * @param[in] val Value to be written.
 * @param[in] len Length of the value to be written counting the zero-terminator.
 * @retval 0 - Success.
 * @retval -EINVAL - Invalid argument.
 */
extern int bk_str_write_raw(struct bk_str *str, const char *val, size_t len);

/**
 * Writes a zero-terminated value to the string handle @p str. All values previously written are kept.
 * @param[in] str String handle.
 * @param[in] val Value to be written.
 * @retval 0 - Success.
 * @retval -EINVAL - Invalid argument.
 */
extern int bk_str_write(struct bk_str *str, const char *val);

/**
 * Reads a zero-terminated value from the string handle @p str.
 * @param[in] str String handle.
 * @param[out] val Value to be read.
 * @param[in,out] len Pointer to specify then store the value length counting the zero-terminator.
 * @retval 0 - Success.
 * @retval -EINVAL - Invalid argument.
 * @retval -ENOBUFS - No buffer space available.
 */
extern int bk_str_read_raw(struct bk_str *str, char *val, size_t *len);

extern int bk_str_read(struct bk_str *str, char *val);

/**
 * Gets the string content from handle @p str.
 * @param[in] str String handle.
 * @return Content as static string.
 * @retval NULL When Invalid argument.
 */
extern const char *bk_str_content(struct bk_str *str);

/**
 * Gets the total string length written in the handle @p str.
 * @param[in] str String handle.
 * @param[out] len Pointer to store the string length.
 * @retval 0 - Success.
 * @retval -EINVAL - Invalid argument.
 */
extern int bk_str_length(struct bk_str *str, size_t *len);

/**
 * Cleans all written string present in the handle @p str.
 * @param[in] str String handle.
 * @retval 0 - Success.
 * @retval -EINVAL - Invalid argument.
 */
extern int bk_str_clear(struct bk_str *str);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* BROOK_H */