#include "pkm.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <memory>

void PackageManager::addPackage(const Package& pkg) {
  packages.emplace(pkg.name, pkg);
}

bool PackageManager::checkDependencies(
    const Package& pkg, std::shared_ptr<std::vector<Package>> pkgInstallList) {
  if (!pkgInstallList) {
    pkgInstallList = std::make_shared<std::vector<Package>>();
  }

  bool resolved = true;

  auto version_checker = [](const std::string &pkg1,const std::string &pkg2,const VersionCompareIdentifier &compare_identifier) -> bool{
    auto result = comparePkgVersion(pkg1,pkg2);

    switch (compare_identifier) {
      case VersionCompareIdentifier::GREATOR_OR_EQUAL:
        if (result == VersionCompareIdentifier::GREATOR || result == VersionCompareIdentifier::EQUAL)
          return true;
        else
          return false;
        break;
      case VersionCompareIdentifier::SMALLER_OR_EQUAL:
        if (result == VersionCompareIdentifier::SMALLER || result == VersionCompareIdentifier::EQUAL)
          return true;
        else
          return false;
        break;
      default:
        if (result == compare_identifier)
          return true;
        else
          return false;
    }
  };
  auto check_deps = [=, &pkgInstallList](bool& resolved) -> void {
    for (const auto& dep : pkg.dependencies) {
      if (packages.count(std::get<0>(dep))) {
        // Required deps is installed, checking it
        auto this_dep = packages.at(std::get<0>(dep));
        if (this_dep.status == PackageStatus::INSTALLED) {
          // Checking dependency version is available
          if (version_checker(this_dep.version, std::get<2>(dep), std::get<1>(dep)))
            continue;
          else
            resolved = false;
        } else {
          // Checking dependency version is available
          if (!version_checker(this_dep.version, std::get<2>(dep), std::get<1>(dep))) {
            resolved = false;
            continue;
          } else {
            if (this->checkDependencies(this_dep, pkgInstallList)) {
              continue;
            } else {
              resolved = false;
            }
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
