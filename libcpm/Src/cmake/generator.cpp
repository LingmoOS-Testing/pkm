/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：generator
 *  Author：Elysia
 *  Date: 25-6-22
 *  Modify Record:
 */

#include "cmake/generator.h"
#include "cmake/cmakelist_template_p.h"

#include "fmt/base.h"
#include "fmt/color.h"

#include<fstream>


namespace cpkm::cmake {
using namespace cpkm::json::config;

bool generateCMakeLists(const std::string& filepath,
                        const PackageConfig<builder::cmake>& config) {
  std::fstream f;
  f.open(filepath,std::ios::out);

  f << fmt::format(CPKM_CMAKE_MINIMUM_VERSION, config.builder.minimumVersion);
  config.version.empty() ? f << fmt::format(CPKM_CMAKE_PROJECT_CONFIG, config.projectName, "")
                         : f << fmt::format(CPKM_CMAKE_PROJECT_CONFIG, config.projectName, "VERSION " + config.version);

  for (auto& target : config.targets) {
    assert(!target.sources.empty());
    std::string sources = {};
    for (auto& source : target.sources) {
      sources += " " + source;
    }
    f << fmt::format(CPKM_CMAKE_ADD_EXECUTABLE, target.name, sources);
  }
  f.close();
  return true;
}
}