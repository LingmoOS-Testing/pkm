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
  // MUST have this entry
  std::string version;
  // MUST have this entry
  std::string resolved;
  // MUST have this entry
  std::string integrity;
  // i.e. this is a synonym for another  package, called as linked to a package.
  // if this is not empty, other fields are ignored and find in linked package instead.
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
  j.at("name").get_to(p.packageName);
  j.at("version").get_to(p.version);
  j.at("lockfileVersion").get_to(p.lockfileVersion);

  // read dependencies
  std::map<std::string, Package> dependencies = {};
  for (auto& [name, pkg] : j.at("dependencies").items()) {
    Package this_pkg = {};
    if (!pkg.contains("linkedTo") || to_string(pkg.at("linkedTo")).empty()) {
      assert(pkg.contains("version"));
      assert(pkg.contains("resolved"));
      assert(pkg.contains("integrity"));
      pkg.at("version").get_to(this_pkg.version);
      pkg.at("resolved").get_to(this_pkg.resolved);
      pkg.at("integrity").get_to(this_pkg.integrity);
      pkg.contains("dev") ? pkg.at("dev").get_to(this_pkg.dev) : this_pkg.dev = false;
      pkg.contains("optional") ? pkg.at("optional").get_to(this_pkg.optional) : this_pkg.optional = false;
      pkg.contains("devOptional") ? pkg.at("devOptional").get_to(this_pkg.devOptional) : this_pkg.devOptional = false;
      // Default to auto for integrity
      pkg.contains("installType") ? pkg.at("installType").get_to(this_pkg.installType) : this_pkg.installType = "auto";
      pkg.contains("hasControlScripts") ? pkg.at("hasControlScripts").get_to(this_pkg.hasControlScripts) : this_pkg.hasControlScripts = false;
      pkg.contains("license") ? pkg.at("license").get_to(this_pkg.license) : this_pkg.license = "";
      // TODO: Fix engines and optionalDependencies when needed!
      this_pkg.engines = {};
      this_pkg.optionalDependencies = {};
      this_pkg.dependencies = {};
      pkg.contains("dependencies") ? pkg.at("dependencies").get_to(this_pkg.dependencies) : this_pkg.dependencies = {};
    } else {
      pkg.at("linkedTo").get_to(this_pkg.linkedTo);
    }
    dependencies[name] = this_pkg;
  }
  p.dependencies = dependencies;
}
}

#endif //PACKAGE_JSON_H