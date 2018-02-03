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

set(B4R_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(B4R_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${B4R_INCLUDE_DIR})

add_library(brook SHARED
        ${B4R_INCLUDE_DIR}/brook.h # allows many IDEs to find it
        ${B4R_SOURCE_DIR}/b4r_macros.h
        ${B4R_SOURCE_DIR}/b4r_utils.c)

if (BUILD_TESTING)
    enable_testing()
    set(B4R_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(test)
endif ()