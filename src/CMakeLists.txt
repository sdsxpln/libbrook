if (MINGW)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(B4R_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_library(brook SHARED
        ${B4R_INCLUDE_DIR}/brook.h # allows many IDEs to find it
        ${B4R_SOURCE_DIR}/b4r_macros.h
        ${B4R_SOURCE_DIR}/b4r_utils.c)