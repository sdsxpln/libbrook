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

set(_system_name "${CMAKE_SYSTEM} ${CMAKE_SYSTEM_PROCESSOR}")
if (NOT ${_system_name} MATCHES ${CMAKE_C_PLATFORM_ID})
    string(CONCAT _system_name ${_system_name} " (${CMAKE_C_PLATFORM_ID})")
endif ()

string(TOUPPER "${CMAKE_BUILD_TYPE}" _BUILD_TYPE)
string(CONCAT _cflags "${CMAKE_C_FLAGS_${_BUILD_TYPE}}" "${CMAKE_C_FLAGS}")
unset(_BUILD_TYPE)

if (CMAKE_SIZEOF_VOID_P MATCHES "8")
    if (MSVC)
        set(_build_arch "x64")
    else ()
        set(_build_arch "x86_64")
    endif ()
else ()
    if (MSVC)
        set(_build_arch "x86")
    else ()
        set(_build_arch "i686")
    endif ()
endif ()

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
    if (${_build_html} MATCHES "Yes")
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
endif ()

if (BUILD_TESTING)
    set(_build_testing "Yes")
    if (BK_TESTS)
        string(CONCAT _build_testing ${_build_testing} " (${BK_TESTS})")
        string(REPLACE ";" ", " _build_testing "${_build_testing}")
    endif ()
else ()
    set(_build_testing "No")
    if (NOT ${_build_type} MATCHES "[Dd]ebug|DEBUG")
        set(_build_testing "${_build_testing} (disabled by build type: ${_build_type})")
    endif ()
endif ()

message("
Brook library ${VERSION} - building summary:

  Generator: ${CMAKE_GENERATOR}
  Install: ${CMAKE_INSTALL_PREFIX}
  System: ${_system_name}
  Compiler:
    Executable: ${CMAKE_C_COMPILER}
    Version: ${CMAKE_C_COMPILER_VERSION}
    CFLAGS: ${_cflags}
  Build: ${_build_type}-${_build_arch}
  Docs:
    HTML: ${_build_html}
    PDF: ${_build_pdf}
  Run tests: ${_build_testing}
")

unset(_system_name)
unset(_build_arch)
unset(_build_type)
unset(_build_html)
unset(_build_pdf)
unset(_cflags)
unset(_build_testing)