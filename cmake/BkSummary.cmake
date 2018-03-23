#.rst:
# BkSummary
# ---------
#
# Building summary.
#
# Summarizes the building options and prints it on screen.

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

if (__BK_SUMMARY_INCLUDED)
    return()
endif ()
set(__BK_SUMMARY_INCLUDED ON)

set(_system_name "${CMAKE_SYSTEM} ${CMAKE_SYSTEM_PROCESSOR}")
if (NOT ${_system_name} MATCHES ${CMAKE_C_PLATFORM_ID})
    string(CONCAT _system_name ${_system_name} " (${CMAKE_C_PLATFORM_ID})")
endif ()

string(TOUPPER "${CMAKE_BUILD_TYPE}" _BUILD_TYPE)
string(CONCAT _cflags "${CMAKE_C_FLAGS_${_BUILD_TYPE}}" " ${CMAKE_C_FLAGS}")
string(REPLACE "  " " " _cflags "${_cflags}")
unset(_BUILD_TYPE)

if (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(_build_arch "x86_64")
else ()
    set(_build_arch "i686")
endif ()

if (CMAKE_BUILD_TYPE)
    set(_build_type ${CMAKE_BUILD_TYPE})
else ()
    set(_build_type "None")
endif ()

if (BUILD_SHARED_LIBS)
    set(_lib_type "shared")
else ()
    set(_lib_type "static")
endif ()

if (${_build_type} MATCHES "[Dd]ebug|DEBUG")
    set(_is_debug ON)
endif ()

if (${_build_type} MATCHES "[Rr]elease|RELEASE")
    set(_is_release ON)
endif ()

if (BK_BUILD_HTML AND BK_GENERATE_HTML)
    if (_is_release)
        set(_build_html "Yes")
    else ()
        set(_build_html "No (unavailable in build type: ${_build_type})")
    endif ()
else ()
    set(_build_html "No")
endif ()

if (BK_BUILD_PDF AND BK_GENERATE_PDF)
    if (_is_release)
        set(_build_pdf "Yes")
        if (${_build_html} MATCHES "Yes")
            if (NOT DOXYGEN_PDFLATEX_EXECUTABLE)
                set(_missing_tool "pdflatex")
            elseif (NOT DOXYGEN_MAKEINDEX_EXECUTABLE)
                set(_missing_tool "makeindex")
            endif ()
            if (_missing_tool)
                set(_build_pdf "No (missing tool: ${_missing_tool})")
            endif ()
        else ()
            set(_build_pdf "No (disabled by \"Build HTML: ${_build_html}\")")
        endif ()
    else ()
        set(_build_pdf "No (unavailable in build type: ${_build_type})")
    endif ()
else ()
    set(_build_pdf "No")
endif ()

if (BK_BUILD_MAN_PAGES AND BK_GENERATE_MAN_PAGES)
    set(_build_man_pages "Yes")
else ()
    set(_build_man_pages "No")
endif ()

if (BK_BUILD_EXAMPLES)
    set(_build_examples "Yes")
    if (BK_EXAMPLES)
        string(CONCAT _build_examples ${_build_examples} " (${BK_EXAMPLES})")
        string(REPLACE ";" ", " _build_examples "${_build_examples}")
    endif ()
else ()
    set(_build_examples "No")
endif ()

if (BUILD_TESTING)
    if (_is_debug)
        set(_build_testing "Yes")
        if (BK_TESTS)
            string(CONCAT _build_testing ${_build_testing} " (${BK_TESTS})")
            string(REPLACE ";" ", " _build_testing "${_build_testing}")
        endif ()
    else ()
        set(_build_testing "${_build_testing} (unavailable in build type: ${_build_type})")
    endif ()
else ()
    set(_build_testing "No")
endif ()

message("
Brook library ${VERSION} - building summary:

  Generator: ${CMAKE_GENERATOR}
  Install: ${CMAKE_INSTALL_PREFIX}
  System: ${_system_name}
  Compiler:
    Executable: ${CMAKE_C_COMPILER}
    Version: ${CMAKE_C_COMPILER_VERSION}
    Machine: ${CMAKE_C_MACHINE}
    CFLAGS: ${_cflags}
  Build: ${_build_type}-${_build_arch} (${_lib_type})
  Examples: ${_build_examples}
  Docs:
    HTML: ${_build_html}
    PDF: ${_build_pdf}
    man pages: ${_build_man_pages}
  Run tests: ${_build_testing}
")

unset(_system_name)
unset(_build_arch)
unset(_build_type)
unset(_build_html)
unset(_build_pdf)
unset(_cflags)
unset(_build_testing)