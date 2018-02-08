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

if (WIN32)
    if (MSVC)
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    endif ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(BK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_library(brook
        ${BK_INCLUDE_DIR}/brook.h # allows many IDEs to find it
        ${BK_SOURCE_DIR}/bk_macros.h
        ${BK_SOURCE_DIR}/bk_utils.c)
if (BUILD_SHARED_LIBS)
    set_target_properties(brook PROPERTIES
            PUBLIC_HEADER ${BK_INCLUDE_DIR}/brook.h
            VERSION ${VERSION}
            SOVERSION ${SOVERSION})
    if (WIN32)
        set_target_properties(brook PROPERTIES
                SUFFIX ${DLL_SUFFIX})
    endif ()
endif ()