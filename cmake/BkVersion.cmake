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

if (EXISTS ${BK_INCLUDE_DIR}/brook.h)
    set(bk_version_list MAJOR MINOR PATCH)
    foreach (v ${bk_version_list})
        set(regex_bk_version "^#define BK_VERSION_${v} ([0-9])")
        file(STRINGS "${BK_INCLUDE_DIR}/brook.h" bk_version_${v} REGEX "${regex_bk_version}")
        string(REGEX REPLACE "${regex_bk_version}" "\\1" bk_version_${v} "${bk_version_${v}}")
        unset(regex_bk_version)
    endforeach ()
    set(VERSION ${bk_version_MAJOR}.${bk_version_MINOR}.${bk_version_PATCH})
    set(BK_VERSION_STRING "${VERSION}")
    foreach (v bk_version_list)
        unset(bk_version_${v})
    endforeach ()
    unset(bk_version_list)
endif ()