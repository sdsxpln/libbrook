#.rst:
# BkSource
# --------
#
# Library source.
#
# This script allows to build the library source and adds the targets to install it as static or shared library.
#
# ::
#
#   BK_SOURCE_DIR - Directory containing the library source.
#   BK_C_SOURCE - All pure C available source.
#   BK_SOURCE - All available source.

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

if (__BK_SRC_INCLUDED)
    return()
endif ()
set(__BK_SRC_INCLUDED ON)

if (WIN32)
    if (MSVC)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    endif ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(BK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

list(APPEND BK_C_SOURCE
        ${BK_SOURCE_DIR}/bk_utils.c
        ${BK_SOURCE_DIR}/bk_str.c)
set(BK_C_SOURCE ${BK_C_SOURCE} PARENT_SCOPE)

list(APPEND BK_SOURCE
        ${BK_INCLUDE_DIR}/brook.h # allows many IDEs to find it
        ${BK_SOURCE_DIR}/bk_macros.h
        ${BK_C_SOURCE})
set(BK_SOURCE ${BK_SOURCE} PARENT_SCOPE)

add_library(brook ${BK_SOURCE})
if (BUILD_SHARED_LIBS)
    set_target_properties(brook PROPERTIES
            PUBLIC_HEADER ${BK_INCLUDE_DIR}/brook.h
            VERSION ${VERSION}
            SOVERSION ${SOVERSION})
    if (WIN32 AND NOT MSVC)
        set_target_properties(brook PROPERTIES
                SUFFIX ${VERSION_SUFFIX}.dll)
    endif ()
endif ()
if (MSVC AND MSVC_VERSION GREATER 1800)
    target_compile_definitions(brook PUBLIC _NO_CRT_STDIO_INLINE)
endif ()

install(TARGETS brook
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT lib # Windows DLL
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT dev)