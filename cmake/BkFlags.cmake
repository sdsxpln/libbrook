#.rst:
# BkFlags
# -------
#
# Compiler/linker warnings.
#
# Configures the compiler/linker flags.
#
# ::
#
#   BK_PICKY_COMPILER - Enable/disable the compiler warnings.

#    _____   _____    _____   _____   _   __
#   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
#   | |_) | | |_) |  | | | | | | | | | |/ /
#   |  _ <  |  _ <   | | | | | | | | |   (
#   | |_) | | | \ \  | |_| | | |_| | | |\ \
#   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\
#
#   –– a small library which helps you write quickly REST APIs.
#
# Copyright (c) 2012-2018 Silvio Clecio <silvioprog@gmail.com>
#
# This file is part of Brook library.
#
# Brook library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Brook library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Brook library.  If not, see <http://www.gnu.org/licenses/>.
#

#TODO: -fsanitize=address/leak

if (__BK_FLAGS_INCLUDED)
    return()
endif ()
set(__BK_FLAGS_INCLUDED ON)

option(BK_PICKY_COMPILER "Enable picky compiler options" ON)

if (BK_PICKY_COMPILER)
    if ((NOT CMAKE_COMPILER_IS_GNUCC) OR (NOT CMAKE_COMPILER_IS_CLANG))
        message(FATALERROR "Unknown C compiler: ${CMAKE_C_COMPILER}")
    else ()
        #-Wsign-conversion
        #-Wpadded
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wpedantic -Wdeclaration-after-statement -Wstrict-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wc90-c99-compat")
        if (ARM)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
        endif ()
    endif ()
endif ()