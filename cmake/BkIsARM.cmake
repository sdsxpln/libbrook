#.rst:
# BkIsArm
# -------
#
# Building for ARM.
#
# Try to detect if the building is for ARM.
#
# ::
#
#   ARM - Is ARM compiler?

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

# It was tested only using `arm-linux-androideabi-gcc`. Be aware!

#TODO: MSVC support

if (__BK_IS_ARM_INCLUDED)
    return()
endif ()
set(__BK_IS_ARM_INCLUDED ON)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
    list(APPEND _list
            ${CMAKE_C_COMPILER}
            ${CMAKE_SYSTEM_PROCESSOR})
    string(TOUPPER "${_list}" _list)
    string(FIND "${_list}" "ARM" _is_arm)
    unset(_list)
    if (${_is_arm} GREATER -1)
        set(ARM ON)
        unset(_is_arm)
    endif ()
endif ()