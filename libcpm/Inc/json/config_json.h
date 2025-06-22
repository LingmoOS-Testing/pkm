/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：package_json, here defines the data structure of the package.json file
 *  Author：Elysia
 *  Date: 25-6-21
 *  Modify Record:
 */
#ifndef CONFIG_JSON_H
#define CONFIG_JSON_H

#include <string>
#include <list>
#include <map>

#include "nlohmann/json.hpp"
#include "common_utils.h"

namespace cpkm::json::config {
namespace builder {
struct _builderConfig {
  std::string type;
  std::string version;
};

struct cmake : _builderConfig {
  std::string minimumVersion;
  std::string cFlags;
  std::string cxxFlags;
  std::string ldFlags;
  std::string cxxStandard;
  std::string cStandard;
};
}

namespace target {
//  target type, can be changed later.
enum class targetType {
  EXECUTABLE = 0x01,
  STATIC_LIBRARY = 0x02,
  SHARED_LIBRARY = 0x04,
  UNKNOWN = 0xFF
};

struct targetConfig {
  std::string name;
  targetType type;
  std::list<std::string> sources;
  std::list<std::string> includes;
};
}

template <typename T>
struct PackageConfig {
  std::string projectName;
  std::string version;
  T builder;
  std::list<std::string> globalIncludes;
  std::list<target::targetConfig> targets;
  std::map<std::string, std::string> dependencies;
};

inline void to_json(nlohmann::json& j, const PackageConfig<builder::cmake>& p) {
  // prepare builder
  std::map<std::string, std::string> builder;
  builder["type"] = p.builder.type;
  builder["version"] = p.builder.version;
  builder["cmakeMinimumRequired"] = p.builder.minimumVersion;
  builder["c_flags"] = p.builder.cFlags;
  builder["c_standard"] = p.builder.cStandard;
  builder["cxx_flags"] = p.builder.cxxFlags;
  builder["cxx_standard"] = p.builder.cxxStandard;
  builder["ld_flags"] = p.builder.ldFlags;

  // prepare targets
  std::list<nlohmann::json> targets;
  for (const auto& [name, type, sources, includes] : p.targets) {
    std::string target_type;
    switch (type) {
      case target::targetType::EXECUTABLE:
        target_type = "executable";
        break;
      case target::targetType::SHARED_LIBRARY:
        target_type = "shared_library";
        break;
      case target::targetType::STATIC_LIBRARY:
        target_type = "static_library";
        break;
      default:
        target_type = "unknown";
    }
    auto targetJson = nlohmann::json{
        {"name", name},
        {"type", target_type},
        {"sources", sources},
        {"includes", includes}
    };
    targets.push_back(targetJson);
  }

  j = nlohmann::json{
      {"project", p.projectName},
      {"version", p.version},
      {"builder", builder},
      {"includes", p.globalIncludes},
      {"targets", targets},
      {"dependencies", p.dependencies}
  };
}

inline void from_json(const nlohmann::json& j, PackageConfig<builder::cmake>& p) {
  using namespace cpkm::utils;
  j.at("project").get_to(p.projectName);
  j.at("version").get_to(p.version);
  j.at("dependencies").get_to(p.dependencies);
  j.at("includes").get_to(p.globalIncludes);

  // prepare builder
  p.builder = {
      j.at("builder").at("type").get<std::string>(),
      j.at("builder").at("version").get<std::string>(),
      j.at("builder").at("cmakeMinimumRequired").get<
        std::string>(),
      j.at("builder").at("c_flags").get<std::string>(),
      j.at("builder").at("cxx_flags").get<std::string>(),
      j.at("builder").at("ld_flags").get<std::string>(),
      j.at("builder").at("cxx_standard").get<std::string>(),
      j.at("builder").at("c_standard").get<std::string>()
  };

  // prepare targets
  std::list<target::targetConfig> targets = {};
  for (const auto& target : j.at("targets")) {
    target::targetConfig t;
    switch (str2tag(target.at("type").get<std::string>())) {
      case "executable"_tag:
        t.type = target::targetType::EXECUTABLE;
        break;
      case "shared_library"_tag:
        t.type = target::targetType::SHARED_LIBRARY;
        break;
      case "static_library"_tag:
        t.type = target::targetType::STATIC_LIBRARY;
        break;
      default:
        t.type = target::targetType::UNKNOWN;
    }
    target.at("name").get_to(t.name);
    target.at("sources").get_to(t.sources);
    target.at("includes").get_to(t.includes);
    targets.push_back(t);
  }
  p.targets = targets;
}
}
template struct cpkm::json::config::PackageConfig<cpkm::json::config::builder::cmake>;
#endif //CONFIG_JSON_H