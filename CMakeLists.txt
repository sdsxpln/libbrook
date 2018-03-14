#.rst:
# BkLibrary
# ---------
#
# Main library building.
#
# The main building of the Brook library. It includes all necessary sub-bulding scripts to manage the library building.
#
# ::
#
#   BK_INCLUDE_DIR - Directory containing the library header.

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

cmake_minimum_required(VERSION 3.5)

project(brook C)

set(CMAKE_C_STANDARD 99)

set(PROJECT_DESCRIPTION "–– a small library which helps you write quickly REST APIs.")

set(PROJECT_VENDOR "The Brook Library Development Team")

set(PROJECT_URL "https://github.com/risoflora/libbrook")

set(PROJECT_ISSUES_URL "${PROJECT_URL}/issues")

set(BK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_COMPILER_IS_CLANG ON)
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(BkIsARM)
include(BkFlags)
include(BkGNUSource)
include(BkVersion)
include(BkPC)
include(BkUninstall)

if (CMAKE_BUILD_TYPE MATCHES "[Rr]elease|RELEASE")
    set(BUILD_TESTING OFF)
elseif (BUILD_TESTING)
    enable_testing()
endif ()

if (UNIX AND (NOT APPLE))
    include(CheckIncludeFiles)
    check_include_files(errno.h HAVE_ERRNO_H)
    if (NOT HAVE_ERRNO_H)
        include_directories(/usr/include/asm-generic)
    endif ()
endif ()
include_directories(${BK_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)

include(BkDoxygen)
include(BkSummary)
include(BkCPack)
include(BkPVSStudio)