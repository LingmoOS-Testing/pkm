/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：package_json
 *  Author：Elysia
 *  Date: 25-6-21
 *  Modify Record:
 */
#ifndef PACKAGE_JSON_H
#define PACKAGE_JSON_H

#include <string>
#include <list>
#include <map>

#include "nlohmann/json.hpp"

namespace cpkm::json::package {
struct Package {
  std::string version;
  std::string resolved;
  std::string integrity;
  std::string linkedTo;
  // dev, optional, devOptional: If the package is strictly part of the devDependencies tree, then dev will be true. If it is strictly part of the optionalDependencies tree, then optional will be set. If it is both a dev dependency and an optional dependency of a non-dev dependency, then devOptional will be set. (An optional dependency of a dev dependency will have both dev and optional set.)
  // We do not have dev and optional implementation yet.
  bool dev;
  bool optional;
  bool devOptional;
  // This indicates the installation type, either "hold", "manual" or "auto".
  std::string installType;
  // This indicates whether the package has control scripts, like preinstall, install, or postinstall
  bool hasControlScripts;
  std::string license;
  // TODO: Should we have this?
  std::map<std::string, std::string> engines;
  std::map<std::string, std::string> dependencies;
  std::map<std::string, std::string> optionalDependencies;
};

struct PackageLock {
  std::string packageName;
  std::string version;
  std::string lockfileVersion;
  std::map<std::string, Package> dependencies;
};

inline void to_json(nlohmann::json& j, const PackageLock& p) {
  // prepare dependencies
  std::map<std::string, nlohmann::json> dependencies;
  for (auto& [name, pkg] : p.dependencies) {
    dependencies[name] = {
        {"version", pkg.version},
        {"resolved", pkg.resolved},
        {"integrity", pkg.integrity},
        {"linkedTo", pkg.linkedTo},
        {"dev", pkg.dev},
        {"optional", pkg.optional},
        {"installType", pkg.installType},
        {"hasControlScripts", pkg.hasControlScripts},
        {"license", pkg.license},
        {"engines", pkg.engines},
        {"dependencies", pkg.dependencies},
        {"optionalDependencies", pkg.optionalDependencies}
    };
  }
  j = nlohmann::json{
      {"name", p.packageName},
      {"version", p.version},
      {"lockfileVersion", p.lockfileVersion},
      {"dependencies", dependencies},
  };
}

inline void from_json(const nlohmann::json& j, PackageLock& p) {
}
}

#endif //PACKAGE_JSON_H