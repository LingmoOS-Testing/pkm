/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：test_cmakelists_generate
 *  Author：Elysia
 *  Date: 25-6-22
 *  Modify Record:
 */
#include <gtest/gtest.h>

#include "cmake/generator.h"
#include "json/config_json.h"

#include <fstream>

TEST(cpkm_test_cmakelists_generate, TestSimpleTask) {
  using namespace cpkm::json::config;
  using namespace cpkm::cmake;
  std::string file_dir = CPKM_TEST_DIR + std::string("/assets/PKGConfigs.json");
  std::string output_dir = CPKM_CMAKE_BINARY_DIR + std::string("/CMakeLists.txt");
  std::ifstream f(file_dir);

  nlohmann::json data = nlohmann::json::parse(f);
  PackageConfig<builder::cmake> p = data.get<PackageConfig<builder::cmake>>();

  generateCMakeLists(output_dir, p);
}
