#    _____   _____    _____   _____   _   __
#   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
#   | |_) | | |_) |  | | | | | | | | | |/ /
#   |  _ <  |  _ <   | | | | | | | | |   (
#   | |_) | | | \ \  | |_| | | |_| | | |\ \
#   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
#
#   –– a small library which helps you write quickly REST APIs.
#
# Copyright (c) 2012-2018 Silvio Clecio, et al.
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

cmake_minimum_required(VERSION 3.5)

project(brook C)

set(CMAKE_C_STANDARD 99)

if (CMAKE_BUILD_TYPE MATCHES "[Rr]elease|RELEASE")
    set(RELEASE ON)
    unset(DEBUG)
else ()
    set(DEBUG ON)
    unset(RELEASE)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(WarnAll)
include(GNUSource)

if (MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif ()

set(B4R_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${B4R_INCLUDE_DIR})

add_subdirectory(src)

if (BUILD_TESTING AND DEBUG)
    enable_testing()
    add_subdirectory(test)
endif ()

if (RELEASE)
    include(DoxygenAPIRef)
endif ()