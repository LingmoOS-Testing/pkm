/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: pkm.hpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: Package Manager's Class Implementation
 * Others:
 * Function List:
 * History:
 *   Elysia 24/07/31 0.0.1 Create this file.
 *************************************************/

#include "pkm.hpp"

#include <cctype>
#include <cstdio>
#include <memory>

void PackageManager::addPackage(const Package& pkg) {
  packages.emplace(pkg.name, pkg);
}

bool PackageManager::checkDependencies(
    const Package& pkg, std::shared_ptr<std::vector<Package>> pkgInstallList,
    std::shared_ptr<std::vector<PackageError>> errorLists) {
  if (!pkgInstallList) {
    pkgInstallList = std::make_shared<std::vector<Package>>();
  }

  if (!errorLists) {
    errorLists = std::make_shared<std::vector<PackageError>>();
  }

  bool resolved = true;

  auto temp_package = Package(pkg);

  // Check current package status
  switch (pkg.status) {
    case PackageStatus::INSTALLED:
      // ToDo: Checking recursively
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);
      break;

    case PackageStatus::UNINSTALLED:
      // If current dep is not install,
      // we may want to test if it can be install <_<
      temp_package.status = PackageStatus::TOINSTALL;

      resolved =
          this->checkDependencies(temp_package, pkgInstallList, errorLists);
      break;

    case PackageStatus::TOINSTALL:
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);

      if (resolved) {
        pkgInstallList->emplace_back(pkg);
      }
      break;
  }

  return resolved;
}

bool PackageManager::m_pkgVersionChecker(
    const std::string& pkg1, const std::string& pkg2,
    const VersionCompareIdentifier& compare_identifier) {
  auto result = comparePkgVersion(pkg1, pkg2);

  switch (compare_identifier) {
    case VersionCompareIdentifier::GREATOR_OR_EQUAL:
      if (result == VersionCompareIdentifier::GREATOR ||
          result == VersionCompareIdentifier::EQUAL)
        return true;
      else
        return false;
      break;
    case VersionCompareIdentifier::SMALLER_OR_EQUAL:
      if (result == VersionCompareIdentifier::SMALLER ||
          result == VersionCompareIdentifier::EQUAL)
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
}
void PackageManager::m_checkPackageStatus(
    bool& resolved, const Package& pkg,
    const std::shared_ptr<std::vector<Package>>& pkgInstallList,
    const std::shared_ptr<std::vector<PackageError>>& errorLists) {
  // Iterate through every required dependencies
  for (const auto& dep : pkg.dependencies) {
    // Checking the required package is in our package cache list
    if (packages.count(std::get<0>(dep)) > 0) {
      // Get the package information from cache
      auto this_dep = packages.at(std::get<0>(dep));
      if (this_dep.status == PackageStatus::INSTALLED) {
        // Required deps is installed, checking it

        // Checking dependency version is available
        if (m_pkgVersionChecker(this_dep.version, std::get<2>(dep),
                                std::get<1>(dep))) {
          continue;  // Have required version install, continue.
        } else {
          // ToDo: Try to test whether it can be upgraded or downgrade
          auto err =
              PackageError{pkg, dep, this_dep,
                           PackageError::ErrorType::DEPENDENCY_NOT_MATCH};
          errorLists->emplace_back(err);
          resolved = false;  // Installed version doesn't meet requirement
        }
      } else {
        // Checking dependency version is available
        if (!m_pkgVersionChecker(this_dep.version, std::get<2>(dep),
                                 std::get<1>(dep))) {
          // If the package is not install and we don't have the required
          // version Raise error
          auto err =
              PackageError{pkg, dep, this_dep,
                           PackageError::ErrorType::DEPENDENCY_NOT_MATCH};
          errorLists->emplace_back(err);
          resolved = false;
          continue;
        } else {
          // Otherwise, we need to check the dependency of this package
          // recursively
          if (this->checkDependencies(this_dep, pkgInstallList)) {
            // If the sub-dependency is resolved, we can continue
            continue;
          } else {
            resolved = false;
          }
        }
        resolved = false;
      }
    } else {
      // ToDo: Check locally install package
      auto err = PackageError{pkg, dep,
                              Package{std::get<0>(dep), std::get<2>(dep), {}},
                              PackageError::ErrorType::DEPENDENCY_NOT_FOUND};
      errorLists->emplace_back(err);
      resolved = false;
    }
  }
};