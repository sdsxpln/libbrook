add_executable(test_utils
        ${B4R_TESTS_DIR}/b4r_assert.h
        ${B4R_TESTS_DIR}/test_utils.c)
target_link_libraries(test_utils brook)
add_test(test_utils test_utils)