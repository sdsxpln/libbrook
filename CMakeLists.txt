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

set(B4R_VERSION_MAJOR 0)
set(B4R_VERSION_MINOR 0)
set(B4R_VERSION_PATCH 1)
set(B4R_VERSION "${B4R_VERSION_MAJOR}.${B4R_VERSION_MINOR}.${B4R_VERSION_PATCH}")
set(B4R_VERSION_NUMBER 0x000001)
add_definitions(-DB4R_VERSION=\"${B4R_VERSION}\")
add_definitions(-DB4R_VERSION_NUMBER=\"${B4R_VERSION_NUMBER}\")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(B4R_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(B4R_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(brook SHARED
        ${B4R_INCLUDE_DIR}/brook.h # allows many IDEs to find it
        ${B4R_SOURCE_DIR}/b4r_macros.h
        ${B4R_SOURCE_DIR}/b4r_utils.c)
