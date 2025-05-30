enable_testing()

file(GLOB_RECURSE TEST_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(cpkm_test ${TEST_SRC_FILES})
target_link_libraries(cpkm_test cpkm_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cpkm_test)