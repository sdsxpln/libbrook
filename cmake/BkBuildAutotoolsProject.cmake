#.rst:
# BkBuildAutotoolsProject
# -----------------------
#
# Builds an autotools project.
#
# Allows to build a project using autotools building system.
#
# ::
#
# Provides:
#
#   build_autotools_project(NAME projectName
#           DIR projectDir
#           CONFIG_OPTIONS configureOptions
#           [QUIET])
#
# Example:
#
#   build_autotools_project(NAME myproject
#           DIR ${CMAKE_SOURCE_DIR}/myproject
#           CONFIG_OPTIONS "-static" "--with-pic")

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

if (__BK_BUILD_AUTOTOOLS_PROJECT_INCLUDED)
    return()
endif ()
set(__BK_BUILD_AUTOTOOLS_PROJECT_INCLUDED ON)

include(CMakeParseArguments)

function(build_autotools_project)
    set(_options QUIET)
    set(_args NAME DIR)
    set(_multi_args CONFIG_OPTIONS)
    cmake_parse_arguments("" "${_options}" "${_args}" "${_multi_args}" ${ARGN})
    unset(_options)
    unset(_args)
    if ("${_NAME}" STREQUAL "")
        message(FATAL_ERROR "NAME should not be empty.")
    endif ()
    if ("${_DIR}" STREQUAL "")
        message(FATAL_ERROR "DIR should not be empty.")
    endif ()
    set(_build_dir "${_DIR}/build")
    file(REMOVE_RECURSE ${_build_dir})
    file(MAKE_DIRECTORY ${_build_dir})
    if (NOT _QUIET)
        message(STATUS "Configuring ${_NAME}")
    endif ()
    set(_configure "../configure")
    set(_prefix ${CMAKE_BINARY_DIR}/${_NAME})
    execute_process(COMMAND ${_configure} --prefix=${_prefix} --host=${CMAKE_C_MACHINE} ${_CONFIG_OPTIONS}
            WORKING_DIRECTORY ${_build_dir}
            ERROR_VARIABLE _error
            RESULT_VARIABLE _result
            OUTPUT_QUIET)
    if (NOT _result EQUAL 0)
        file(REMOVE_RECURSE ${_DIR})
        message(FATAL_ERROR "\
Error configuring ${_NAME}
${_error}")
    endif ()
    unset(_result)
    unset(_error)
    unset(_configure)
    if (NOT _QUIET)
        message(STATUS "Configuring ${_NAME} - done")
    endif ()
    if (NOT _QUIET)
        message(STATUS "Building ${_NAME}")
    endif ()
    execute_process(COMMAND ${CMAKE_MAKE_PROGRAM}
            WORKING_DIRECTORY ${_build_dir}
            ERROR_VARIABLE _error
            RESULT_VARIABLE _result
            OUTPUT_QUIET)
    if (NOT _result EQUAL 0)
        message(FATAL_ERROR "\
Error building ${_NAME}
${_error}")
    endif ()
    unset(_result)
    unset(_error)
    if (NOT _QUIET)
        message(STATUS "Building ${_NAME} - done")
    endif ()
    if (NOT _QUIET)
        message(STATUS "Installing ${_NAME}")
    endif ()
    set(_install_args install)
    if (${CMAKE_BUILD_TYPE} MATCHES "[Rr]elease|RELEASE")
        set(_install_args ${_install_args}-strip)
    endif ()
    execute_process(COMMAND ${CMAKE_MAKE_PROGRAM} ${_install_args}
            WORKING_DIRECTORY ${_build_dir}
            ERROR_VARIABLE _error
            RESULT_VARIABLE _result
            OUTPUT_QUIET)
    unset(_install_args)
    if (NOT _result EQUAL 0)
        message(FATAL_ERROR "\
Error installing ${_NAME}
${_error}")
    endif ()
    unset(_prefix)
    unset(_result)
    unset(_error)
    if (NOT _QUIET)
        message(STATUS "Installing ${_NAME} - done")
    endif ()
    unset(_build_dir)
    unset(_DIR)
endfunction()