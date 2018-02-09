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

option(BK_BUILD_HTML "Generate API reference [HTML]" ON)
option(BK_BUILD_PDF "Generate API reference [PDF]" ${BK_BUILD_HTML})

if (BK_BUILD_HTML)
    find_package(Doxygen QUIET)
    if (DOXYGEN_FOUND)
        set(BK_GENERATE_HTML ON)
        set(DOXYGEN_INPUT_FILE ${CMAKE_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUTPUT_FILE ${CMAKE_BINARY_DIR}/Doxyfile)
        set(DOXYGEN_DOC_DIR ${CMAKE_BINARY_DIR}/doc)
        set(BK_GENERATE_PDF NO)
        if (BK_BUILD_PDF)
            find_program(DOXYGEN_PDFLATEX_EXECUTABLE pdflatex)
            if (DOXYGEN_PDFLATEX_EXECUTABLE)
                find_program(DOXYGEN_MAKEINDEX_EXECUTABLE makeindex)
                if (DOXYGEN_MAKEINDEX_EXECUTABLE)
                    set(BK_GENERATE_PDF YES)
                else ()
                    message(WARNING "makeindex required to generate the API reference PDF")
                endif ()
            else ()
                message(WARNING "pdflatex required to generate the API reference PDF")
            endif ()
        endif ()
        configure_file(${DOXYGEN_INPUT_FILE} ${DOXYGEN_OUTPUT_FILE} @ONLY)
        message(STATUS "Generating Doxygen file - done")
        add_custom_target(doc ALL
                COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_DOC_DIR}
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUTPUT_FILE}
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating API reference with Doxygen [HTML]"
                VERBATIM)
        if (BK_GENERATE_PDF)
            set(DOXYGEN_LATEX_DIR ${DOXYGEN_DOC_DIR}/latex)
            add_custom_target(pdf ALL
                    COMMAND ${CMAKE_MAKE_PROGRAM} -C ${DOXYGEN_LATEX_DIR}
                    COMMAND ${CMAKE_COMMAND} -E rename ${DOXYGEN_LATEX_DIR}/refman.pdf ${DOXYGEN_LATEX_DIR}/libbrook-v${VERSION}.pdf
                    WORKING_DIRECTORY ${DOXYGEN_LATEX_DIR}
                    COMMENT "Generating API reference with Doxygen [PDF]"
                    DEPENDS doc
                    VERBATIM)
        endif ()
    else ()
        message(WARNING "Doxygen required to generate the API reference [HTML/PDF]")
    endif ()
endif ()