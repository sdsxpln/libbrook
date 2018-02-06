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

if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(B4R_TESTS_DIR ${CMAKE_SOURCE_DIR}/test)

option(BUILD_UTILS_TESTING "Build b4r_utils testing" ON)

if (BUILD_UTILS_TESTING)
    add_executable(test_utils
            ${B4R_TESTS_DIR}/b4r_assert.h
            ${B4R_TESTS_DIR}/test_utils.c)
    if (MSVC)
        target_compile_definitions(test_utils PUBLIC
                _CRT_STDIO_INLINE=static
                _CRT_SECURE_NO_WARNINGS)
    endif ()
    target_link_libraries(test_utils brook)
    add_test(NAME test_utils
            WORKING_DIRECTORY ${B4R_TESTS_DIR}
            COMMAND test_utils)
endif ()