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
 * \mainpage
 * \li \ref bk_api
 */

/** \defgroup bk_api API reference
 * The API reference grouped by feature.
 * */

#ifndef BROOK_H
#define BROOK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>

#define BK_VERSION_MAJOR 0
#define BK_VERSION_MINOR 0
#define BK_VERSION_PATCH 1
#define BK_VERSION_HEX ((BK_VERSION_MAJOR << 16) | (BK_VERSION_MINOR <<  8) | (BK_VERSION_PATCH))

/**
 * \ingroup bk_api
 * \defgroup bk_utils Utilities
 * All utility functions of the library.
 * \{
 */

/**
 * Returns the library version number.
 * \return Library version packed into a single integer.
 */
extern unsigned int bk_version(void);

/**
 * Returns the library version number as string.
 * \return Library version packed into a null-terminated static string.
 */
extern const char *bk_version_str(void);

/**
 * Allocates a new zero-initialize memory space.
 * \param[in] size Memory size to be allocated.
 * \return Pointer of the zero-initialized allocated memory.
 * \warning By default it exits the application when no memory space available.
 */
extern void *bk_alloc(size_t size);

/**
 * Frees a memory space previous allocated by #bk_alloc().
 * \param[in] ptr Pointer of the memory to be freed.
 */
extern void bk_free(void *ptr);

/** \} */

/**
 * \ingroup bk_api
 * \defgroup bk_str String
 * String handle and its related functions.
 * \{
 */

/**
 * Handle for the string structure used to represent a HTML body, POST payload and more.
 * \struct bk_str
 */
struct bk_str;

/**
 * Creates a new zero-initialized string instance.
 * \return String instance.
 * \retval NULL When no memory space available.
 */
extern struct bk_str *bk_str_new(void);

/**
 * Frees a string instance previous allocated by #bk_str_new().
 * \param[in] str Pointer of the string instance to be freed.
 */
extern void bk_str_free(struct bk_str *str);

/**
 * Writes a C string to the string handle \p str. All strings previously written are kept.
 * \param[in] str String handle.
 * \param[in] val String to be written.
 * \param[in] len Length of the string to be written.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
extern int bk_str_write(struct bk_str *str, const char *val, size_t len);

/**
 * Reads a C string from the string handle \p str.
 * \param[in] str String handle.
 * \param[out] val String to be read.
 * \param[in,out] len Pointer to specify then store the string length.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOBUFS - No buffer space available to store the string.
 */
extern int bk_str_read(struct bk_str *str, char *val, size_t *len);

/**
 * Writes a formatted C string from variable argument list to the string handle \p str.
 * \param[in] str String handle.
 * \param[in] fmt Formatted string (following the same [`printf()`](https://linux.die.net/man/3/printf) format
 *  specification).
 * \param[in] ap Arguments list variable (handled by
 *  [`va_start()`](https://linux.die.net/man/3/va_start)/[`va_end()`](https://linux.die.net/man/3/va_end)).
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
extern int bk_str_printf_va(struct bk_str *str, const char *fmt, va_list ap);

/**
 * Writes a formatted C string to the string handle \p str. All strings previously written are kept.
 * \param[in] str String handle.
 * \param[in] fmt Formatted string (following the same [`printf()`](https://linux.die.net/man/3/printf) format
 *  specification).
 * \param[in] ... Additional arguments (following the same [`printf()`](https://linux.die.net/man/3/printf) arguments
 *  specification).
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
extern int bk_str_printf(struct bk_str *str, const char *fmt, ...);

/**
 * Gets the C string content from the string handle \p str.
 * \param[in] str String handle.
 * \return Content as static null-terminated string.
 * \retval NULL When Invalid argument.
 */
extern const char *bk_str_content(struct bk_str *str);

/**
 * Gets the total string length written in the handle \p str.
 * \param[in] str String handle.
 * \param[out] len Pointer to store the string length.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
extern int bk_str_length(struct bk_str *str, size_t *len);

/**
 * Cleans all written string present in the string handle \p str.
 * \param[in] str String handle.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
extern int bk_str_clear(struct bk_str *str);

/** \} */

/**
 * \ingroup bk_api
 * \defgroup bk_strmap String map
 * String map handle and its related functions.
 * \{
 */

/**
 * Handle for the hash table that maps pairs of strings. Each pair contains a name and a value. It is useful to
 * represent a HTML field, a query-string parameter and more.
 * \struct bk_strmap
 */
struct bk_strmap;

/**
 * Callback signature used by #bk_strmap_iter() to iterate pairs of name-value.
 * \param[out] cls User-defined closure.
 * \param[out] pair Current iterated pair.
 */
typedef int (*bk_strmap_iter_cb)(void *cls, struct bk_strmap *pair);

/**
 * Callback signature used by #bk_strmap_sort() to sort pairs of `name-value`.
 * \param[out] cls User-defined closure.
 * \param[out] pair_a Current left pair (A).
 * \param[out] pair_b Current right pair (B).
 */
typedef int (*bk_strmap_sort_cb)(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b);

/**
 * Returns the name from the \p pair.
 * \param[in] pair Pair of name-value.
 * \return Name as null-terminated string.
 * \retval NULL When no memory space available.
 */
extern const char *bk_strmap_name(struct bk_strmap *pair);

/**
 * Returns the value from the \p pair.
 * \param[in] pair Pair of name-value.
 * \return Value as null-terminated string.
 * \retval NULL When no memory space available.
 */
extern const char *bk_strmap_val(struct bk_strmap *pair);

/**
 * Reads the name from the \p pair.
 * \param[in] pair Pair of name-value.
 * \param[out] name Name to be read.
 * \param[in,out] len Pointer to specify then store the name length.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOBUFS - No buffer space available to store the name.
 */
extern int bk_strmap_readname(struct bk_strmap *pair, char *name, size_t *len);

/**
 * Reads the value from the \p pair.
 * \param[in] pair Pair of name-value.
 * \param[out] val Value to be read.
 * \param[in,out] len Pointer to specify then store the value length.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOBUFS - No buffer space available to store the value.
 */
extern int bk_strmap_readval(struct bk_strmap *pair, char *val, size_t *len);

/**
 * Adds a pair of name-value to the list \p map.
 * \param[in,out] map Pointer of the list to add the new pair.
 * \param[in] name Name of the pair to be added.
 * \param name_len Length of the \p name.
 * \param val Value of the pair to be added.
 * \param val_len Length of the \p value.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOMEM - No memory available available.
 * \note This function does not check if a name already exists in a pair added to the \p map, then the uniqueness must
 * be managed by the application.
 */
extern int bk_strmap_add(struct bk_strmap **map, const char *name, size_t name_len, const char *val, size_t val_len);

/**
 * Sets a pair of name-value to the list \p map.
 * \param[in,out] map Pointer of the list to set the new pair.
 * \param[in] name Name of the pair to be set.
 * \param name_len Length of the \p name.
 * \param val Value of the pair to be set.
 * \param val_len Length of the \p value.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOMEM - No memory available available.
 * \note When a name already exists in pair previously added into the \p map, then the function replaces its value,
 * otherwise it is added as a new pair.
 */
extern int bk_strmap_set(struct bk_strmap **map, const char *name, size_t name_len, const char *val, size_t val_len);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern int bk_strmap_find(struct bk_strmap *map, const char *name, size_t len, struct bk_strmap **pair);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern int bk_strmap_rm(struct bk_strmap **map, const char *name, size_t len);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern int bk_strmap_iter(struct bk_strmap *map, bk_strmap_iter_cb iter_cb, void *iter_cls);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern int bk_strmap_sort(struct bk_strmap **map, bk_strmap_sort_cb cmp_cb, void *cmp_cls);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern int bk_strmap_count(struct bk_strmap *map, unsigned int *count);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern int bk_strmap_next(struct bk_strmap **map);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
extern void bk_strmap_cleanup(struct bk_strmap **map);

/** \} */

#ifdef __cplusplus
}
#endif

#endif /* BROOK_H */
