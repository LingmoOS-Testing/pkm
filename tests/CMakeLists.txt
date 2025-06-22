enable_testing()

add_compile_options(-DCPKM_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}" -DCPKM_CMAKE_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")
link_libraries(cpkm_lib GTest::gtest_main)

add_executable(cpkm_test_deps test_normal_deps.cpp)
add_executable(cpkm_test_json_parser test_json_parser.cpp)
add_executable(cpkm_test_cmakelists_generate test_cmakelists_generate.cpp)

include(GoogleTest)
gtest_discover_tests(cpkm_test_deps cpkm_test_json_parser)