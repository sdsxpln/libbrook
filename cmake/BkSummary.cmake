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

if (CMAKE_BUILD_TYPE)
    set(_build_type ${CMAKE_BUILD_TYPE})
else ()
    set(_build_type "None")
endif ()

if (BK_BUILD_HTML AND BK_GENERATE_HTML)
    set(_build_html "Yes")
else ()
    set(_build_html "No")
    if (NOT ${_build_type} MATCHES "[Rr]elease|RELEASE")
        set(_build_html "${_build_html} (disabled by build type: ${_build_type})")
    endif ()
endif ()

if (BK_BUILD_PDF AND BK_GENERATE_PDF)
    set(_build_pdf "Yes")
else ()
    set(_build_pdf "No")
    if (${_build_type} MATCHES "[Rr]elease|RELEASE")
        if (NOT DOXYGEN_PDFLATEX_EXECUTABLE)
            set(_missing_tool "pdflatex")
        elseif (NOT DOXYGEN_MAKEINDEX_EXECUTABLE)
            set(_missing_tool "makeindex")
        endif ()
        set(_build_pdf "${_build_pdf} (missing ${_missing_tool} tool)")
    else ()
        set(_build_pdf "${_build_pdf} (disabled by build type: ${_build_type})")
    endif ()
endif ()

string(TOUPPER "${CMAKE_BUILD_TYPE}" _BUILD_TYPE)
string(CONCAT _cflags "${CMAKE_C_FLAGS_${_BUILD_TYPE}}" "${CMAKE_C_FLAGS}")

message("
Brook library ${VERSION} - Build options summary:

  Install prefix: ${CMAKE_INSTALL_PREFIX}
  System name: ${CMAKE_SYSTEM_NAME}
  System processor: ${CMAKE_SYSTEM_PROCESSOR}
  Compiler options:
    C compiler: ${CMAKE_C_COMPILER}
    CFLAGS: ${_cflags}
    Build type: ${_build_type}
  Build docs:
    HTML: ${_build_html}
    PDF: ${_build_pdf}
")

unset(_build_type)
unset(_BUILD_TYPE)
unset(_build_html)
unset(_build_pdf)
unset(_cflags)