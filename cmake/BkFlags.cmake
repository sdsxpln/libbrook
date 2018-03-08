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

if (ARM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif ()

if (BK_PICKY_COMPILER)
    if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
        #-Wsign-conversion - needs to fix (un)signed bugs in utstring.h
        #-Wpadded - depends on https://github.com/troydhanson/uthash/pull/151
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wdeclaration-after-statement -Wstrict-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline")
        if (NOT ARM)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic")
            if (CMAKE_COMPILER_IS_GNUCC)
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wc90-c99-compat")
            endif ()
        endif ()
    else ()
        message(FATAL_ERROR "Unknown C compiler: ${CMAKE_C_COMPILER}")
    endif ()
endif ()