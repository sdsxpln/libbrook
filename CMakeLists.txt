#    _____   _____    _____   _____   _   __
#   |  _  \ |  _  \  /  _  \ /  _  \ | | / /
#   | |_) | | |_) |  | | | | | | | | | |/ /
#   |  _ <  |  _ <   | | | | | | | | |   (
#   | |_) | | | \ \  | |_| | | |_| | | |\ \
#   |_____/ |_|  \_\ \_____/ \_____/ |_| \_\ 4-REST.
#
#   –– a small tool which helps you write quickly REST APIs.
#
# Copyright (c) 2012-2017 Silvio Clecio, et al.
#
# This software is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at https://brookframework.org/docs/copyright.html.
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the COPYING file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#

# *** WARNING: The Brook cmake build system is poorly maintained. Be aware. ***

cmake_minimum_required(VERSION 3.6)

project(brook C)

# standard sets

set(CMAKE_C_STANDARD 99)

# build type

if (WIN32)
    set(CMAKE_C_FLAGS "-static -Wno-nonnull-compare")
else ()
    set(CMAKE_C_FLAGS "")
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(DEBUG 1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fsanitize=address")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

# options

option(ENABLE_HTTPS_SUPPORT "Enable HTTPS support" OFF)

# includes

include(GNUInstallDirs)
include(CheckSymbolExists)

# sets

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

set(UTH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/uthash/src)

set(MHD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libmicrohttpd)

set(MHD_INC_DIR ${MHD_DIR} ${MHD_DIR}/src/include)

set(MHD_LIB_DIR ${MHD_DIR}/src/microhttpd/.libs)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(EXP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)

set(TST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(LIB_FILES
        ${INC_DIR}/brook.h
        ${SRC_DIR}/md5.c
        ${SRC_DIR}/b4r_macros.h
        ${SRC_DIR}/b4r_httpsrv_strs.h
        ${SRC_DIR}/b4r_hs.c
        ${SRC_DIR}/b4r_utils.c
        ${SRC_DIR}/b4r_httpsrv_cfg.c
        ${SRC_DIR}/b4r_httpsrv_res_cache.c
        ${SRC_DIR}/b4r_httpsrv_req_uplds.c
        ${SRC_DIR}/b4r_httpsrv_req.c
        ${SRC_DIR}/b4r_httpsrv_res.c
        ${SRC_DIR}/b4r_httpsrv.c
        ${SRC_DIR}/b4r_httpsrv_auth.c
        ${SRC_DIR}/b4r_router.c)

# includes directories

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${UTH_DIR})
include_directories(${MHD_INC_DIR})
include_directories(${INC_DIR})
include_directories(${SRC_DIR})

# defs

CHECK_SYMBOL_EXISTS(__GNU_LIBRARY__ "features.h" _GNU_SOURCE)

if (_GNU_SOURCE)
    add_definitions(-D_GNU_SOURCE)
endif ()

if (MINGW)
    add_definitions(-DMINGW=1)
endif ()

# libraries

set(LIBS ${MHD_LIB_DIR}/libmicrohttpd.a)

if (UNIX)
    # libuuid on Ubuntu 32 bits is located at "/lib/i386-linux-gnu/libuuid.so.1.3.0"
    set(LIBS ${LIBS} uuid pthread)
elseif (WIN32)
    set(LIBS ${LIBS} rpcrt4 ws2_32)
endif ()

if (ENABLE_HTTPS_SUPPORT)
    set(LIBS ${LIBS} gnutls gcrypt)
endif ()

# shared

add_library(brook SHARED ${LIB_FILES})
target_compile_definitions(brook PUBLIC -DB4R_SHARED_LIBRARY=1)
target_link_libraries(brook ${LIBS})

# static

add_library(brook_static STATIC ${LIB_FILES})
target_compile_definitions(brook_static PUBLIC -DB4R_STATIC_LIBRARY=1)
set_target_properties(brook_static PROPERTIES OUTPUT_NAME brook)

set(LIBS ${LIBS} brook)

# examples

add_executable(hs ${EXP_DIR}/hs.c)
target_link_libraries(hs ${LIBS})

add_executable(helloworld_httpsrv ${EXP_DIR}/helloworld_httpsrv.c)
target_link_libraries(helloworld_httpsrv ${LIBS})

add_executable(httpsrv_errs ${EXP_DIR}/httpsrv_errs.c)
target_link_libraries(httpsrv_errs ${LIBS})

add_executable(httpsrv_upld ${EXP_DIR}/httpsrv_upld.c)
target_link_libraries(httpsrv_upld ${LIBS})

# tests

#add_executable(test_cfg ${TST_DIR}/test_cfg.c)
#target_link_libraries(test_cfg ${LIBS})