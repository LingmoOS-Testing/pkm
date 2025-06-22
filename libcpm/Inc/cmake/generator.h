/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：generator
 *  Author：Elysia
 *  Date: 25-6-22
 *  Modify Record:
 */	
#ifndef CMAKE_GENERATOR_H
#define CMAKE_GENERATOR_H

#include "json/config_json.h"

#include <string>

namespace cpkm::cmake {
  using namespace json::config;
  bool generateCMakeLists(const std::string& filepath, const PackageConfig<builder::cmake>& config);
}

#endif //CMAKE_GENERATOR_H
