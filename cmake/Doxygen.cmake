#    _____   _____    _____   _____   _   __
#   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
#   | |_) | | |_) |  | | | | | | | | | |/ /
#   |  _ <  |  _ <   | | | | | | | | |   (
#   | |_) | | | \ \  | |_| | | |_| | | |\ \
#   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\
#
#   –– a small library which helps you write quickly REST APIs.
#
# Copyright (c) 2012-2018 Silvio Clecio.
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

option(BUILD_DOC "Build documentation" ON)

if (BUILD_DOC)
    find_package(Doxygen QUIET)
    if (DOXYGEN_FOUND)
        set(DOXYGEN_INPUT_FILE ${CMAKE_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc)
        set(DOXYGEN_OUTPUT_FILE ${CMAKE_BINARY_DIR}/Doxyfile)
        configure_file(${DOXYGEN_INPUT_FILE} ${DOXYGEN_OUTPUT_FILE} @ONLY)
        message(STATUS "Generating Doxygen file - done")
        add_custom_target(doc ALL
                COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUTPUT_FILE}
                #TODO: rename all refman occurrences with libbrook-v0.0.1
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating API reference with Doxygen [HTML]"
                VERBATIM)
        add_custom_target(pdf ALL
                COMMAND ${CMAKE_MAKE_PROGRAM} -C ${DOXYGEN_OUTPUT_DIR}/latex
                WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}/latex
                COMMENT "Generating API reference with Doxygen [PDF]"
                DEPENDS doc
                VERBATIM)
    else ()
        message(WARNING "Doxygen need to be installed to generate the doxygen documentation")
    endif ()
endif ()