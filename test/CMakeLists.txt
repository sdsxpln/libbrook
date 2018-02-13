#.rst:
# BkTests
# -------
#
# Library testing.
#
# All library testings.
#
# ::
#
#   BK_TESTS_DIR - Directory containing the library testings.
#   BK_TESTS - All available testings.
#   BK_TESTS_SOURCE - All available source.
#   BK_BUILD_<TEST>_TESTING - Enable/disable a <TEST> listed by BK_TESTS, e.g: -DBK_BUILD_UTILS_TESTING=ON.

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

if (BUILD_TESTING)
    if (WIN32)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    endif ()
    set(BK_TESTS_DIR ${CMAKE_SOURCE_DIR}/test)
    list(APPEND BK_TESTS
            utils
            str)
    set(BK_TESTS ${BK_TESTS} PARENT_SCOPE)
    foreach (_test ${BK_TESTS})
        string(TOUPPER ${_test} _TEST)
        option(BK_BUILD_${_TEST}_TESTING "Build bk_${_test} testing" ON)
        if (BK_BUILD_${_TEST}_TESTING)
            list(APPEND BK_TESTS_SOURCE ${BK_TESTS_DIR}/test_${_test}.c)
            add_executable(test_${_test}
                    ${BK_TESTS_DIR}/bk_assert.h
                    ${BK_TESTS_DIR}/test_${_test}.c)
            if (MSVC)
                target_compile_definitions(test_${_test} PUBLIC
                        _CRT_STDIO_INLINE=static
                        _CRT_SECURE_NO_WARNINGS)
            endif ()
            target_link_libraries(test_${_test} brook)
            add_test(NAME test_${_test}
                    WORKING_DIRECTORY ${BK_TESTS_DIR}
                    COMMAND test_${_test})
        endif ()
        unset(_TEST)
    endforeach ()
    set(BK_TESTS_SOURCE ${BK_TESTS_SOURCE} PARENT_SCOPE)
endif ()