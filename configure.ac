#    _____   _____    _____   _____   _   __
#   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
#   | |_) | | |_) |  | | | | | | | | | |/ /
#   |  _ <  |  _ <   | | | | | | | | |   (
#   | |_) | | | \ \  | |_| | | |_| | | |\ \
#   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
#
#   –– a small tool which helps you write quickly REST APIs.
#
# Copyright (c) 2012-2017 Silvio Clecio, et al.
#
# This file is part of Brook4-REST.
#
# Brook4-REST is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Brook4-REST is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Brook4-REST.  If not, see <http://www.gnu.org/licenses/>.
#

AC_PREREQ([2.69])
AC_INIT([libbrook], [0.0.0], [https://github.com/silvioprog/brook4rest/issues])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_GNU_SOURCE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([memory.h stdbool.h inttypes.h stddef.h stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([floor memmove memset stpcpy strdup strerror strtoumax])

AC_OUTPUT
