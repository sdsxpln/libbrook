#.rst:
# BkDoxygen
# ---------
#
# Doxygen variables for the Brook library documentation generation.
#
# Adds the targets `make html` and `make pdf` to generate respectively the HTML and PDF API reference of
# the Brook library.
#
# ::
#
#   BK_BUILD_HTML - Enable/disable the API reference generation as HTML.
#   BK_BUILD_PDF - Enable/disable the API reference generation as PDF. Note: it depends on BK_BUILD_HTML=ON.
#   BK_BUILD_MAN_PAGES - Enable/disable the API reference man pages generation. Note: it depends on BK_BUILD_HTML=ON.
#   BK_GENERATE_HTML - True when enabled the API reference generation as HTML.
#   BK_GENERATE_PDF - True when there is the possibility to generating the API reference as PDF.
#   BK_GENERATE_MAN_PAGES - True when there is the possibility to generating the API reference as man pages.
#
#   DOXYGEN_FOUND - True when Doxygen executable is found.
#   DOXYGEN_INPUT_FILE - Template file used to generate the DOXYGEN_OUTPUT_FILE.
#   DOXYGEN_OUTPUT_FILE - Production file to be used as Doxygen input file. It is generated from the
#                         template DOXYGEN_INPUT_FILE.
#   DOXYGEN_DOC_DIR - Directory containing the generated HTML files.
#   DOXYGEN_PDFLATEX_EXECUTABLE - The path of the `pdflatex` executable when it is found.
#   DOXYGEN_MAKEINDEX_EXECUTABLE - The path of the `makeindex` executable when it is found.
#   DOXYGEN_LATEX_DIR - Directory containing the generated PDF file.
#   DOXYGEN_MAN_DIR - Directory containing the generated man page files.

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

#TODO: Graphviz

if (__BK_DOXYGEN_INCLUDED OR (NOT CMAKE_BUILD_TYPE MATCHES "[Rr]elease|RELEASE"))
    return()
endif ()
set(__BK_DOXYGEN_INCLUDED ON)

option(BK_BUILD_HTML "Generate API reference [HTML]" OFF)
option(BK_BUILD_PDF "Generate API reference [PDF]" ${BK_BUILD_HTML})
option(BK_BUILD_MAN_PAGES "Generate API reference [man pages]" ${BK_BUILD_HTML})

if (BK_BUILD_HTML)
    find_package(Doxygen QUIET)
    if (DOXYGEN_FOUND)
        set(BK_GENERATE_HTML ON)
        set(DOXYGEN_INPUT_FILE ${CMAKE_SOURCE_DIR}/doxygen/Doxyfile.in)
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
        if (BK_BUILD_MAN_PAGES)
            set(BK_GENERATE_MAN_PAGES YES)
            set(DOXYGEN_MAN_DIR ${DOXYGEN_DOC_DIR}/man)
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
            add_custom_target(pdf
                    COMMAND ${CMAKE_COMMAND} -E chdir ${DOXYGEN_LATEX_DIR} ${CMAKE_BUILD_TOOL}
                    COMMAND ${CMAKE_COMMAND} -E rename ${DOXYGEN_LATEX_DIR}/refman.pdf ${DOXYGEN_LATEX_DIR}/libbrook-v${VERSION}.pdf
                    WORKING_DIRECTORY ${DOXYGEN_LATEX_DIR}
                    COMMENT "Generating API reference with Doxygen [PDF]"
                    DEPENDS doc
                    VERBATIM)
        endif ()
        if (BK_GENERATE_MAN_PAGES)
            foreach (_src ${BK_C_SOURCE})
                get_filename_component(_filename ${_src} NAME_WE)
                set(_filename ${DOXYGEN_MAN_DIR}/man3/${_filename}.3)
                #TODO: generate .gz files
                install(FILES ${_filename}
                        DESTINATION ${CMAKE_INSTALL_MANDIR}/man3)
                unset(_filename)
            endforeach ()
        endif ()
    else ()
        message(WARNING "Doxygen required to generate the API reference [HTML/PDF]")
    endif ()
endif ()