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
 * @defgroup brook_api Brook library API reference
 * @{
 */

#ifndef BROOK_H
#define BROOK_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && defined(_MSC_VER)
# ifdef brook_EXPORTS
#  define  BK_EXTERN __declspec(dllexport)
# else
#  define BK_EXTERN __declspec(dllimport)
# endif
#endif
#ifndef BK_EXTERN
# define BK_EXTERN
#endif

#define BK_VERSION_MAJOR 0
#define BK_VERSION_MINOR 0
#define BK_VERSION_PATCH 1
#define BK_VERSION_HEX ((BK_VERSION_MAJOR << 16) | (BK_VERSION_MINOR <<  8) | (BK_VERSION_PATCH))

/* utilities */

/**
 * Returns the library version number.
 * @return the library version packed into a single integer.
 */
BK_EXTERN unsigned int bk_version(void);

/**
 * Returns the library version number as string.
 * @return the library version packed into a static string.
 */
BK_EXTERN const char *bk_version_str(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BROOK_H */