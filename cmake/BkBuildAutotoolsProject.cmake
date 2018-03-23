#TODO: this module is experimental, it will be documented as soon as it was done

if (__BK_BUILD_AUTOTOOLS_PROJECT_INCLUDED)
    return()
endif ()
set(__BK_BUILD_AUTOTOOLS_PROJECT_INCLUDED ON)

include(CMakeParseArguments)

function(build_autotools_project)
    set(_options QUIET)
    set(_args NAME DIR)
    set(_multi_args OPTIONS)
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
    execute_process(COMMAND ${_configure} --prefix=${_build_dir} --host=${CMAKE_C_MACHINE} ${_OPTIONS}
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
    set(${_NAME}_BUILD_DIR "${_build_dir}" PARENT_SCOPE)
    unset(_build_dir)
    unset(_DIR)
endfunction()