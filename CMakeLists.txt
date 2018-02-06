cmake_minimum_required(VERSION 3.5)

project(brook C)

set(CMAKE_C_STANDARD 99)

option(ENABLE_PICKY_COMPILER "Enable picky compiler options" ON)

if (ENABLE_PICKY_COMPILER)
    if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra")
    elseif (MSVC)
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Wall /WX")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Wall /WX")
    endif ()
endif ()

include(CheckSymbolExists)

if (NOT _GNU_SOURCE)
    check_symbol_exists(__GNU_LIBRARY__ "features.h" _GNU_SOURCE)

    if (NOT _GNU_SOURCE)
        unset(_GNU_SOURCE CACHE)
        check_symbol_exists(_GNU_SOURCE "features.h" _GNU_SOURCE)
    endif ()
endif ()

if (_GNU_SOURCE)
    add_definitions(-D_GNU_SOURCE)
endif ()

if (MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif ()

set(B4R_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${B4R_INCLUDE_DIR})

add_subdirectory(src)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")

    option(BUILD_DOC "Build documentation" ON)

    if (BUILD_DOC)
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
            set(DOXYGEN_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
            set(DOXYGEN_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
            configure_file(${DOXYGEN_INPUT_FILE} ${DOXYGEN_OUTPUT_FILE} @ONLY)
            message("Doxygen build started")
            add_custom_target(doc ALL
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUTPUT_FILE}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API reference with Doxygen"
                    VERBATIM)
        else (DOXYGEN_FOUND)
            message("Doxygen need to be installed to generate the doxygen documentation")
        endif (DOXYGEN_FOUND)
    endif ()
endif ()