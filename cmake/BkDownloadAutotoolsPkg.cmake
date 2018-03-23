#TODO: this module is experimental, it will be documented as soon as it was done

if (__BK_DOWNLOAD_AUTOTOOLS_PKG_INCLUDED)
    return()
endif ()
set(__BK_DOWNLOAD_AUTOTOOLS_PKG_INCLUDED ON)

include(CMakeParseArguments)

function(download_autotools_package)
    set(_args NAME URL SHA256 DIR)
    set(_options QUIET)
    cmake_parse_arguments("" "${_options}" "${_args}" "" ${ARGN})
    unset(_args)
    unset(_options)
    if ("${_NAME}" STREQUAL "")
        message(FATAL_ERROR "NAME should not be empty.")
    endif ()
    if ("${_URL}" STREQUAL "")
        message(FATAL_ERROR "URL should not be empty.")
    endif ()
    if ("${_SHA256}" STREQUAL "")
        message(FATAL_ERROR "SHA256 should not be empty.")
    endif ()
    if ("${_DIR}" STREQUAL "")
        message(FATAL_ERROR "DIR should not be empty.")
    endif ()
    get_filename_component(_filename ${_URL} NAME)
    if (NOT ${_filename} MATCHES "\\.tar\\.gz$")
        message(FATAL_ERROR "File ${_filename} is not a tar.gz package")
    endif ()
    set(_full_filename "${_DIR}/${_filename}")
    if (NOT _QUIET)
        message(STATUS "Looking for package ${_filename}...")
    endif ()
    if (EXISTS ${_full_filename})
        if (NOT _QUIET)
            message(STATUS "Looking for package ${_filename}... found")
        endif ()
    else ()
        if (NOT _QUIET)
            message(STATUS "Looking for package ${_filename}... not found")
            if (NOT _QUIET)
                message(STATUS "Downloading ${_NAME}...")
            endif ()
            if (EXISTS ${_full_filename})
                set(_status_code 0)
            else ()
                file(DOWNLOAD ${_URL} ${_full_filename}
                        STATUS _status)
            endif ()
            list(GET _status 0 _status_code)
            list(GET _status 1 _status_text)
            unset(_status)
            if (NOT ${_status_code} EQUAL 0)
                file(REMOVE ${_full_filename})
                message(FATAL_ERROR "\
Error downloading ${_NAME} from ${_URL}
${_status_code} - ${_status_text}")
            endif ()
            unset(_status_code)
            unset(_status_text)
            if (NOT _QUIET)
                message(STATUS "Downloading ${_NAME}... done")
            endif ()
        endif ()
    endif ()
    if (NOT _QUIET)
        message(STATUS "Checking ${_NAME}...")
    endif ()
    file(SHA256 ${_full_filename} _sha256)
    if (NOT "${_sha256}" STREQUAL "${_SHA256}")
        file(REMOVE ${_full_filename})
        message(FATAL_ERROR "\
Checksum failed for ${_filename}
Expected: ${_SHA256}
Generated: ${_sha256}")
    endif ()
    unset(_sha256)
    if (NOT _QUIET)
        message(STATUS "Checking ${_NAME}... done")
    endif ()
    if (NOT _QUIET)
        message(STATUS "Extracting ${_NAME}...")
    endif ()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar zxvf ${_full_filename}
            WORKING_DIRECTORY ${_DIR}
            RESULT_VARIABLE _result
            OUTPUT_QUIET)
    unset(_full_filename)
    if (NOT _result EQUAL 0)
        file(REMOVE_RECURSE ${_DIR})
        message(FATAL_ERROR "Error extracting ${_filename}")
    endif ()
    unset(_result)
    if (NOT _QUIET)
        message(STATUS "Extracting ${_NAME}... done")
    endif ()
    string(REPLACE ".tar.gz" "" _name ${_filename})
    unset(_filename)
    set(${_NAME}_DIR "${_DIR}/${_name}" PARENT_SCOPE)
    unset(_name)
    unset(_NAME)
    unset(_URL)
    unset(_SHA256)
    unset(_DIR)
endfunction()