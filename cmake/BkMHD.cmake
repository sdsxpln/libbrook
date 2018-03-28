#.rst:
# BkMHD
# -----
#
# Build libmicrohttpd.
#
# Build libmicrohttpd from Brook building.
#
# ::
#
#   MHD_INCLUDE_DIR - Directory of includes.
#   MHD_ARCHIVE_LIB - AR archive library.
#

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

set(MHD_NAME "libmicrohttpd")
set(MHD_VER "0.9.59")
set(MHD_FULL_NAME "${MHD_NAME}-${MHD_VER}")
set(MHD_URL "https://ftp.gnu.org/gnu/libmicrohttpd/${MHD_FULL_NAME}.tar.gz")
set(MHD_SHA256 "9b9ccd7d0b11b0e179f1f58dc2caa3e0c62c8609e1e1dc7dcaadf941b67d923c")
set(MHD_OPTIONS
        "--enable-static=yes"
        "--enable-shared=no"
        "--enable-messages=yes"
        "--enable-https=no" #TODO: enable by options, something like -DBK_HTTPS_SUPPORT=ON
        "--enable-asserts=no"
        "--enable-coverage=no"
        "--disable-doc"
        "--disable-examples"
        "--disable-curl")

if (MINGW)
    set(_bash bash)
endif ()

ExternalProject_Add(${MHD_FULL_NAME}
        URL ${MHD_URL}
        URL_HASH SHA256=${MHD_SHA256}
        DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/lib
        PREFIX ${CMAKE_BINARY_DIR}/${MHD_FULL_NAME}
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/${MHD_FULL_NAME}
        CONFIGURE_COMMAND ${_bash} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> ${MHD_OPTIONS}
        BUILD_COMMAND ${CMAKE_MAKE_PROGRAM}
        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
        LOG_INSTALL ON)
unset(_bash)

ExternalProject_Get_Property(${MHD_FULL_NAME} INSTALL_DIR)
set(MHD_INCLUDE_DIR ${INSTALL_DIR}/include)
set(MHD_ARCHIVE_LIB ${INSTALL_DIR}/lib/${MHD_NAME}.a)
unset(INSTALL_DIR)