#.rst:
# BkPVSStudio
# -----------
#
# PVS-Studio analysis.
#
# This script allows to check the library sources using the PVS-Studio, offering more security for Brook library users.

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

if (__BK_PVS_STUDIO_INCLUDED)
    return()
endif ()
set(__BK_PVS_STUDIO_INCLUDED ON)

option(BK_PVS_STUDIO "Enable PVS-Studio analysis" OFF)

if (BK_PVS_STUDIO)
    include(PVS-Studio)
    if (NOT CMAKE_EXPORT_COMPILE_COMMANDS)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    endif ()
    pvs_studio_add_target(TARGET pvs_studio_analysis ALL
            FORMAT fullhtml
            ANALYZE ${PROJECT_NAME}
            SOURCES ${BK_SOURCE} ${BK_TESTS_SOURCE}
            LOG pvs_studio_fullhtml)
endif ()