/*    _____   _____    _____   _____   _   __
 *   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
 *   | |_) | | |_) |  | | | | | | | | | |/ /
 *   |  _ <  |  _ <   | | | | | | | | |   (
 *   | |_) | | | \ \  | |_| | | |_| | | |\ \
 *   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
 *
 *   –– a small tool which helps you write quickly REST APIs.
 *
 * Copyright (c) 2012-2018 Silvio Clecio, et al.
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

#ifndef BROOK_H
#define BROOK_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && defined(_MSC_VER)
# ifdef brook_EXPORTS
#  define  B4R_EXTERN __declspec(dllexport)
# else
#  define B4R_EXTERN __declspec(dllimport)
# endif
#endif
#ifndef B4R_EXTERN
# define B4R_EXTERN
#endif

#define B4R_VERSION_MAJOR 0
#define B4R_VERSION_MINOR 0
#define B4R_VERSION_PATCH 1
#define B4R_VERSION_HEX ((B4R_VERSION_MAJOR << 16) | (B4R_VERSION_MINOR <<  8) | (B4R_VERSION_PATCH))

/* utilities */

/**
 * Returns the library version number.
 * @return the library version packed into a single integer.
 */
B4R_EXTERN unsigned int b4r_version(void);

/**
 * Returns the library version number as string.
 * @return the library version packed into a static string.
 */
B4R_EXTERN const char *b4r_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* BROOK_H */