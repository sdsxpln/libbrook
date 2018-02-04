if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(B4R_TESTS_DIR ${CMAKE_SOURCE_DIR}/test)

option(BUILD_UTILS_TESTING "Build b4r_utils testing" ON)

if (BUILD_UTILS_TESTING)
    add_executable(test_utils
            ${B4R_TESTS_DIR}/b4r_assert.h
            ${B4R_TESTS_DIR}/test_utils.c)
    if (MSVC)
        target_compile_definitions(test_utils PUBLIC
                _CRT_STDIO_INLINE=static
                _CRT_SECURE_NO_WARNINGS)
    endif ()
    target_link_libraries(test_utils brook)
    add_test(NAME test_utils
            WORKING_DIRECTORY ${B4R_TESTS_DIR}
            COMMAND test_utils)
endif ()