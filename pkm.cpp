#include "pkm.h"

#include <fstream>
#include <memory>
#include <sstream>

Package::Package(const Package &other) {
  name = other.name;
  version = other.version;
  dependencies = other.dependencies;
  status = other.status;
}


void PackageManager::addPackage(const Package& pkg) {
  packages.emplace(pkg.name, pkg);
}

bool PackageManager::checkDependencies(const Package& pkg, std::shared_ptr<std::vector<Package>> pkgInstallList) {
  if (!pkgInstallList) {
    pkgInstallList = std::make_shared<std::vector<Package>>();
  }

  bool resolved = true;
  auto check_deps = [&, this, pkgInstallList](bool &resolved) -> void {
    for (const auto& dep : pkg.dependencies) {
      if (packages.count(std::get<0>(dep))) {
        // Required deps is installed, checking it
        auto this_dep = packages.at(std::get<0>(dep));
        if (this_dep.status == PackageStatus::INSTALLED) {
          continue;
        } else {
          if (this->checkDependencies(this_dep, pkgInstallList)) {
            continue;
          } else {
            resolved = false;
          }
          resolved = false;
        }
      } else {
        resolved = false;
      }
    }
  };

  auto temp_package = Package(pkg);

  // Check current package status
  switch (pkg.status) {
    case PackageStatus::INSTALLED:
      // ToDo: Checking recursively
      check_deps(resolved);
      break;

    case PackageStatus::UNINSTALLED:
      // If current dep is not install,
      // we may want to test if it can be install <_<
      temp_package.status = PackageStatus::TOINSTALL;

      resolved = this->checkDependencies(temp_package, pkgInstallList);
      break;

    case PackageStatus::TOINSTALL:
      check_deps(resolved);

      if (resolved) {
        pkgInstallList->emplace_back(pkg);
      }
      break;
  }

  return resolved;
}
