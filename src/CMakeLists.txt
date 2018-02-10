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

if (WIN32)
    if (MSVC)
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    endif ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(BK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_library(brook
        ${BK_INCLUDE_DIR}/brook.h # allows many IDEs to find it
        ${BK_SOURCE_DIR}/bk_macros.h
        ${BK_SOURCE_DIR}/bk_utils.c)
if (BUILD_SHARED_LIBS)
    set_target_properties(brook PROPERTIES
            PUBLIC_HEADER ${BK_INCLUDE_DIR}/brook.h
            VERSION ${VERSION}
            SOVERSION ${SOVERSION})
    target_compile_definitions(brook PUBLIC -D_SHARED)
    if (WIN32 AND NOT MSVC)
        set_target_properties(brook PROPERTIES
                SUFFIX ${VERSION_SUFFIX}.dll)
    endif ()
endif ()

include(GNUInstallDirs)

install(TARGETS brook
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (NOT TARGET uninstall)
    configure_file(
            ${CMAKE_MODULE_PATH}/CMakeUninstall.cmake.in
            ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
            IMMEDIATE @ONLY)
    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake)
endif ()