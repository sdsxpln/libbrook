option(BUILD_UTILS_TESTING "Build b4r_utils testing" ON)

if (MINGW)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif ()

set(B4R_TESTS_DIR ${CMAKE_SOURCE_DIR}/test)

if (BUILD_UTILS_TESTING)
    add_executable(test_utils
            ${B4R_TESTS_DIR}/b4r_assert.h
            ${B4R_TESTS_DIR}/test_utils.c)
    target_link_libraries(test_utils brook)
    add_test(NAME test_utils
            WORKING_DIRECTORY ${B4R_TESTS_DIR}
            COMMAND test_utils)
endif ()