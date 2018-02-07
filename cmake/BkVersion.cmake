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

if (EXISTS ${B4R_INCLUDE_DIR}/brook.h)
    set(brook_version_list MAJOR MINOR PATCH)
    foreach (v ${brook_version_list})
        set(regex_brook_version "^#define B4R_VERSION_${v} ([0-9])")
        file(STRINGS "${B4R_INCLUDE_DIR}/brook.h" brook_version_${v} REGEX "${regex_brook_version}")
        string(REGEX REPLACE "${regex_brook_version}" "\\1" brook_version_${v} "${brook_version_${v}}")
        unset(regex_brook_version)
    endforeach ()
    set(BROOK_VERSION_STRING "${brook_version_MAJOR}.${brook_version_MINOR}.${brook_version_PATCH}")
    set(VERSION ${BROOK_VERSION_STRING})
    foreach (v brook_version_list)
        unset(brook_version_${v})
    endforeach ()
    unset(brook_version_list)
endif ()