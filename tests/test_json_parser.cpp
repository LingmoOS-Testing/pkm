/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：test_json_parser
 *  Author：Elysia
 *  Date: 25-6-21
 *  Modify Record:
 */
#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "config_json.h"
#include "package_json.h"
#include "nlohmann/json.hpp"

TEST(cpkm_test_json_project, JSONToNativeTest) {
  using namespace cpkm::json::config;
  std::string file_dir = std::string(CPKM_TEST_DIR) + std::string("/assets/PKGConfigs.json");
  std::ifstream f(file_dir);
  ASSERT_TRUE(f.is_open());

  nlohmann::json data = nlohmann::json::parse(f);
  PackageConfig<builder::cmake> p = data.get<PackageConfig<builder::cmake>>();
  ASSERT_EQ(p.projectName, std::string("cpkm_test_project"));
  ASSERT_EQ(p.version, std::string("1.0.0"));
  ASSERT_EQ(p.builder.type, std::string("cmake"));
  ASSERT_EQ(p.builder.version, std::string("3.10.0"));
  ASSERT_EQ(p.builder.minimumVersion, std::string("2.8.12"));
  ASSERT_EQ(p.builder.cFlags, std::string("-Wall"));
  ASSERT_EQ(p.builder.cxxFlags, std::string("-Wall"));
  ASSERT_EQ(p.builder.ldFlags, std::string("-L/opt/mysql/lib"));
  ASSERT_EQ(p.builder.cStandard, std::string("11"));
  ASSERT_EQ(p.builder.cxxStandard, std::string("17"));
  ASSERT_EQ(p.globalIncludes.size(), 1);
  ASSERT_EQ(p.globalIncludes.front(), std::string("include/*"));
  ASSERT_EQ(p.targets.size(), 1);
  ASSERT_EQ(p.targets.front().name, std::string("MyApp"));
  ASSERT_EQ(p.targets.front().type, target::targetType::EXECUTABLE);
  ASSERT_EQ(p.targets.front().sources.size(), 1);
  ASSERT_EQ(p.targets.front().sources.front(), std::string("Sources/*"));
  ASSERT_EQ(p.targets.front().includes.size(), 1);
  ASSERT_EQ(p.targets.front().includes.front(), std::string("include_p/*"));
  ASSERT_EQ(p.dependencies.size(), 12);
  ASSERT_EQ(p.dependencies.at("foo"), std::string("1.0.0 - 2.9999.9999"));
  ASSERT_EQ(p.dependencies.at("bar"), std::string(">=1.0.2 <2.1.2"));
}

TEST(cpkm_test_json_project, NativeToJSONTest) {
  using namespace cpkm::json::config;
  std::string file_dir = std::string(CPKM_TEST_DIR) + std::string("/assets/PKGConfigs.json");
  std::ifstream f(file_dir);
  ASSERT_TRUE(f.is_open());

  nlohmann::json data = nlohmann::json::parse(f);
  PackageConfig<builder::cmake> p = data.get<PackageConfig<builder::cmake>>();

  // convert back to json
  nlohmann::json j = p;
  ASSERT_EQ(j.at("project"), data.at("project"));
  ASSERT_EQ(j.at("builder"), data.at("builder"));
  ASSERT_EQ(j.at("version"), data.at("version"));
  ASSERT_EQ(j.at("targets"), data.at("targets"));
  ASSERT_EQ(j.at("dependencies"), data.at("dependencies"));
  ASSERT_EQ(j.at("includes"), data.at("includes"));
}

TEST(cpkm_test_json_package, JSONToNativeTest) {
  using namespace cpkm::json::package;
  std::string file_dir = std::string(CPKM_TEST_DIR) + std::string("/assets/PKGLocks.json");
  std::ifstream f(file_dir);
  ASSERT_TRUE(f.is_open());

  nlohmann::json data = nlohmann::json::parse(f);
  PackageLock p = data.get<PackageLock>();
  ASSERT_EQ(p.packageName, std::string("hexo-site"));
}

TEST(cpkm_test_json_package, NativeToJSONTest) {
  using namespace cpkm::json::package;
  std::string file_dir = std::string(CPKM_TEST_DIR) + std::string("/assets/PKGLocks.json");
  std::ifstream f(file_dir);
  ASSERT_TRUE(f.is_open());

  nlohmann::json data = nlohmann::json::parse(f);
  PackageLock p = data.get<PackageLock>();

  // convert back to json
  nlohmann::json j = p;
  ASSERT_EQ(j.at("lockfileVersion"), data.at("lockfileVersion"));
}