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
#include <stdbool.h>
#include <stdarg.h>

#ifndef BK_EXTERN
# ifdef _WIN32
#  ifdef BUILDING_LIBBROOK
#   define BK_EXTERN __declspec(dllexport) extern
#  else
#   define BK_EXTERN __declspec(dllimport) extern
#  endif
# else
#  define BK_EXTERN extern
# endif
#endif

#ifndef __BK_MALLOC
#define __BK_MALLOC __attribute__((malloc))
#endif

#ifndef __BK_FORMAT
#define __BK_FORMAT(...) __attribute__((format(printf, __VA_ARGS__)))
#endif

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
BK_EXTERN unsigned int bk_version(void);

/**
 * Returns the library version number as string.
 * \return Library version packed into a null-terminated static string.
 */
BK_EXTERN const char *bk_version_str(void);

/**
 * Allocates a new zero-initialize memory space.
 * \param[in] size Memory size to be allocated.
 * \return Pointer of the zero-initialized allocated memory.
 * \retval NULL When size is `0` or no memory space.
 */
BK_EXTERN void *bk_alloc(size_t size)
__BK_MALLOC;

/**
 * Frees a memory space previous allocated by #bk_alloc().
 * \param[in] ptr Pointer of the memory to be freed.
 */
BK_EXTERN void bk_free(void *ptr);

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
 * Creates a new zero-initialized string handle.
 * \return String handle.
 * \warning It exits the application when no memory space available.
 */
BK_EXTERN struct bk_str *bk_str_new(void)
__BK_MALLOC;

/**
 * Frees a string handle previous allocated by #bk_str_new().
 * \param[in] str Pointer of the string handle to be freed.
 */
BK_EXTERN void bk_str_free(struct bk_str *str);

/**
 * Copies a null-terminated string to the string handle \p str. All strings previously copied are kept.
 * \param[in] str String handle.
 * \param[in] val String to be copied.
 * \param[in] len Length of the string to be copied.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN int bk_str_strcpy(struct bk_str *str, const char *val, size_t len);

/**
 * Prints a formatted null-terminated string from variable argument list to the string handle \p str.
 * \param[in] str String handle.
 * \param[in] fmt Formatted string (following the same [`printf()`](https://linux.die.net/man/3/printf) format
 *  specification).
 * \param[in] ap Arguments list variable (handled by
 *  [`va_start()`](https://linux.die.net/man/3/va_start)/[`va_end()`](https://linux.die.net/man/3/va_end)).
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN int bk_str_printf_va(struct bk_str *str, const char *fmt, va_list ap);

/**
 * Prints a formatted null-terminated string to the string handle \p str. All strings previously written are kept.
 * \param[in] str String handle.
 * \param[in] fmt Formatted string (following the same [`printf()`](https://linux.die.net/man/3/printf) format
 *  specification).
 * \param[in] ... Additional arguments (following the same [`printf()`](https://linux.die.net/man/3/printf) arguments
 *  specification).
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN int bk_str_printf(struct bk_str *str, const char *fmt, ...)
__BK_FORMAT(2, 3);

/**
 * Returns the null-terminated string content from the string handle \p str.
 * \param[in] str String handle.
 * \return Content as static null-terminated string.
 * \retval NULL When the \p str is `NULL`.
 */
BK_EXTERN const char *bk_str_content(struct bk_str *str);

/**
 * Returns the total string length from the handle \p str.
 * \param[in] str String handle.
 * \return Total string length.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN size_t bk_str_length(struct bk_str *str);

/**
 * Cleans all written string present in the string handle \p str.
 * \param[in] str String handle.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN int bk_str_clear(struct bk_str *str);

/** \} */

/**
 * \ingroup bk_api
 * \defgroup bk_strmap String map
 * String map handle and its related functions.
 * \{
 */

/**
 * Handle for the hash table that maps pairs of null-terminated strings. Each pair contains a name and a value. It is
 * useful to represent a HTML field, a query-string parameter and more.
 * \struct bk_strmap
 */
struct bk_strmap;

/**
 * Callback signature used by #bk_strmap_iter() to iterate pairs of strings.
 * \param[out] cls User-defined closure.
 * \param[out] pair Current iterated pair.
 */
typedef int (*bk_strmap_iter_cb)(void *cls, struct bk_strmap *pair);

/**
 * Callback signature used by #bk_strmap_sort() to sort pairs of strings.
 * \param[out] cls User-defined closure.
 * \param[out] pair_a Current left pair (A).
 * \param[out] pair_b Current right pair (B).
 */
typedef int (*bk_strmap_sort_cb)(void *cls, struct bk_strmap *pair_a, struct bk_strmap *pair_b);

/**
 * Returns the name from the \p pair.
 * \param[in] pair Pair of name-value.
 * \return Name as null-terminated string.
 * \retval NULL When the \p pair is `NULL`.
 */
BK_EXTERN const char *bk_strmap_name(struct bk_strmap *pair);

/**
 * Returns the value from the \p pair.
 * \param[in] pair Pair of name-value.
 * \return Value as null-terminated string.
 * \retval NULL When the \p pair is `NULL`.
 */
BK_EXTERN const char *bk_strmap_val(struct bk_strmap *pair);

/**
 * Adds a pair of null-terminated string to the \p map.
 * \param[in,out] map Pairs map pointer to add a new pair.
 * \param[in] name Name of the pair to be added.
 * \param[in] val Value of the pair to be added.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \note It does not check if a name already exists in a pair added to the \p map, then the uniqueness must be managed
 * by the application.
 * \warning It exits the application when no memory space available.
 */
BK_EXTERN int bk_strmap_add(struct bk_strmap **map, const char *name, const char *val);

/**
 * Sets a pair of null-terminated string to the \p map.
 * \param[in,out] map Pairs map pointer to set a new pair.
 * \param[in] name Name of the pair to be set.
 * \param[in] val Value of the pair to be set.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \note When a name already exists in pair previously added into the \p map, then the function replaces its value,
 * otherwise it is added as a new pair.
 * \warning It exits the application when no memory space available.
 */
BK_EXTERN int bk_strmap_set(struct bk_strmap **map, const char *name, const char *val);

/**
 * Finds a pair by its name.
 * \param[in] map Pairs map.
 * \param[in] name Name of the pair to be found.
 * \param[in,out] pair Pointer to store found pair.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOENT - Pair not found.
 */
BK_EXTERN int bk_strmap_find(struct bk_strmap *map, const char *name, struct bk_strmap **pair);

/**
 * Removes a pair by its name.
 * \param[in] map Pointer to the pairs map.
 * \param[in] name Name of the pair to be removed.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \retval -ENOENT - Pair already removed.
 */
BK_EXTERN int bk_strmap_rm(struct bk_strmap **map, const char *name);

/**
 * Iterates over pairs map.
 * \param[in] map Pairs map.
 * \param[in] cb Callback to iterate the pairs.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 * \return Callback result when it is different from `0`.
 */
BK_EXTERN int bk_strmap_iter(struct bk_strmap *map, bk_strmap_iter_cb cb, void *cls);

/**
 * Sorts the pairs map.
 * \param[in,out] map Pointer to the pairs map.
 * \param[in] cb Callback to sort the pairs.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN int bk_strmap_sort(struct bk_strmap **map, bk_strmap_sort_cb cb, void *cls);

/**
 * Counts the total pairs present in the map.
 * \param[in] map Pairs map.
 * \return Total of pairs.
 * \retval 0 When the list is empty or null.
 */
BK_EXTERN unsigned int bk_strmap_count(struct bk_strmap *map);

/**
 * Returns the next pair in the map.
 * \param[in,out] next Pointer to the next pair.
 * \retval 0 - Success.
 * \retval -EINVAL - Invalid argument.
 */
BK_EXTERN int bk_strmap_next(struct bk_strmap **next);

/**
 * Cleans the entire map.
 * \param[in] map Pointer to the pairs map.
 */
BK_EXTERN void bk_strmap_cleanup(struct bk_strmap **map);

/** \} */

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
struct bk_httpreq;

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
struct bk_httpres;

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
struct bk_httpsrv;

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
typedef void (*bk_httperr_cb)(void *cls, const char *err);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
typedef int (*bk_httpreq_cb)(void *cls, struct bk_httpreq *req, struct bk_httpres *res);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN struct bk_httpsrv *bk_httpsrv_new2(bk_httpreq_cb req_cb, void *req_cls, bk_httperr_cb err_cb, void *err_cls);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN struct bk_httpsrv *bk_httpsrv_new(bk_httpreq_cb cb, void *cls);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN void bk_httpsrv_free(struct bk_httpsrv *srv);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN int bk_httpsrv_start(struct bk_httpsrv *srv, unsigned short port, bool threaded);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN int bk_httpsrv_stop(struct bk_httpsrv *srv);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN int bk_httpres_type(struct bk_httpres *res, const char *type);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN int bk_httpres_status(struct bk_httpres *res, unsigned int status);

/* experimental: it will be documented and tested as soon as it is accepted as better API. */
BK_EXTERN struct bk_str *bk_httpres_body(struct bk_httpres *res);

#ifdef __cplusplus
}
#endif

#endif /* BROOK_H */
