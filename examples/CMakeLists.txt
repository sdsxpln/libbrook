#.rst:
# BkExamples
# ----------
#
# Library examples.
#
# All library examples building.
#
# ::
#
#   BK_EXAMPLES_DIR - Directory containing the library examples.
#   BK_EXAMPLES - All available examples.
#   BK_EXAMPLES_SOURCE - All available example source.
#   BK_BUILD_<EXAMPLE>_EXAMPLE - Enable/disable a <BK_EXAMPLE> listed by BK_BK_EXAMPLES, e.g: -DBK_BUILD_STR_EXAMPLE=ON.

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

if (__BK_EXAMPLES_INCLUDED)
    return()
endif ()
set(__BK_EXAMPLES_INCLUDED ON)

option(BK_BUILD_EXAMPLES "Enable the library examples building" ON)

if (BK_BUILD_EXAMPLES)
    if (WIN32)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    endif ()
    set(BK_EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
    list(APPEND BK_EXAMPLES
            str
            strmap
            httpsrv)
    set(BK_EXAMPLES ${BK_EXAMPLES} PARENT_SCOPE)
    list(APPEND _libs brook)
    foreach (_example ${BK_EXAMPLES})
        string(TOUPPER ${_example} _EXAMPLE)
        option(BK_BUILD_${_EXAMPLE}_EXAMPLE "Build bk_${_example} example" ON)
        if (BK_BUILD_${_EXAMPLE}_EXAMPLE)
            list(APPEND BK_EXAMPLES_SOURCE ${BK_EXAMPLES_DIR}/example_${_example}.c)
            add_executable(example_${_example}
                    ${BK_EXAMPLES_DIR}/example_${_example}.c)
            target_link_libraries(example_${_example} ${_libs})
        endif ()
        unset(_EXAMPLE)
    endforeach ()
    unset(_libs)
    set(BK_EXAMPLES_SOURCE ${BK_EXAMPLES_SOURCE} PARENT_SCOPE)
endif ()