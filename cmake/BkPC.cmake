#.rst:
# BkPC
# ----
#
# `pkg-config` file generation.
#
# Generates and install the `libbrook.pc` file. When it is successfully installed, the `pkg-config` tool can be used to
# get info regarding the library, e.g: `pkg-config libbrook --modversion` returns the library version. More:
# [`pkg-config(1)`](https://linux.die.net/man/1/pkg-config).

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

configure_file(${CMAKE_MODULE_PATH}/libbrook.pc.cmake.in
        ${CMAKE_BINARY_DIR}/libbrook.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/libbrook.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig COMPONENT dev)