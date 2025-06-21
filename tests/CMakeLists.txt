enable_testing()

add_compile_options(-DCPKM_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(cpkm_test_deps test_normal_deps.cpp)
target_link_libraries(cpkm_test_deps cpkm_lib GTest::gtest_main)

add_executable(cpkm_test_json_parser test_json_parser.cpp)
target_link_libraries(cpkm_test_json_parser cpkm_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cpkm_test_deps cpkm_test_json_parser)