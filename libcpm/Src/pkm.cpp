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

#include "log.h"

void PackageManager::addPackage(const Package& pkg) {
  packageCacheList.emplace(pkg.name, pkg);
}

void PackageManager::addlocalInstalledPackage(const Package& pkg) {
  packageInstalledList.emplace(pkg.name, pkg);
}

bool PackageManager::checkDependencies(
    const Package& pkg,
    std::shared_ptr<std::map<std::string, Package>> pkgInstallList,
    std::shared_ptr<std::vector<PackageError>> errorLists) {
  log_debug("Checking package: %s", pkg.name.c_str());

  if (!pkgInstallList) {
    pkgInstallList = std::make_shared<std::map<std::string, Package>>();
  }

  if (!errorLists) {
    errorLists = std::make_shared<std::vector<PackageError>>();
  }

  bool resolved = true;

  // Check current package status
  switch (pkg.status) {
    case PackageStatus::INSTALLED:
      // ToDo: Checking recursively to test dependency, like wether the deps is missing 
      // or we need to update them
      log_debug("Package \"%s\" installed, checking its deps status.", pkg.name.c_str());
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);
      break;

    case PackageStatus::UNINSTALLED:
      // If current dep is not installed,
      // we may want to test if it can be installed <_<
      log_debug("Package \"%s\" is not installed, checking if it can be inst.", pkg.name.c_str());
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);

      if (resolved) {
        log_debug("\tResolved, add %s into install list", pkg.name.c_str());
        pkgInstallList->insert({pkg.name, pkg});
      }
      break;

    case PackageStatus::TOINSTALL:
     log_debug("Requested to install Package \"%s\".", pkg.name.c_str());
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);

      if (resolved) {
        log_debug("\tResolved, add %s into install list", pkg.name.c_str());
        pkgInstallList->insert({pkg.name, pkg});
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
    const std::shared_ptr<std::map<std::string, Package>> pkgInstallList,
    const std::shared_ptr<std::vector<PackageError>>& errorLists) {
  // Iterate through every required dependencies
  for (const auto& dep : pkg.dependencies) {
    // Checking the required package is in our local installed list
    if (packageInstalledList.count(dep.name) > 0) {
      // If we have the package installed locally
      // Checking dependency version is available
      if (auto this_dep = packageInstalledList.at(dep.name);
          m_pkgVersionChecker(this_dep.version, dep.version, dep.compare_id)) {
        continue;  // Have required version install, continue.
      }
    }

    // Check if the package is already analysed and added to the waiting list
    if (pkgInstallList->count(dep.name) > 0) {
      if (auto this_dep = pkgInstallList->at(dep.name);
          m_pkgVersionChecker(this_dep.version, dep.version, dep.compare_id)) {
        continue;  // Have required version install, continue.
      } else {
        // The package is planned to be installed,
        // but the current needed version is not available or conflict
        // with the planned one
        auto err =
            PackageError{pkg, dep, this_dep,
                         PackageError::ErrorType::DEPENDENCY_NOT_INSTALLABLE};
        errorLists->emplace_back(err);
        resolved = false;
        return;
      }
    }

    // Otherwise, we need to check if we can install it
    if (packageCacheList.count(dep.name) > 0) {
      // Get the package information from cache
      // Checking dependency version is available
      if (auto this_dep = packageCacheList.at(dep.name);
          !m_pkgVersionChecker(this_dep.version, dep.version, dep.compare_id)) {
        // If the package is not install and we don't have the required
        // version Raise error
        auto err = PackageError{pkg, dep, this_dep,
                                PackageError::ErrorType::DEPENDENCY_NOT_MATCH};
        errorLists->emplace_back(err);
        resolved = false;
        continue;
      } else {
        // Otherwise, we need to check the dependency of this package
        // recursively
        if (this->checkDependencies(this_dep, pkgInstallList, errorLists)) {
          // If the sub-dependency is resolved, we can continue
          continue;
        } else {
          auto err =
              PackageError{pkg, dep, this_dep,
                           PackageError::ErrorType::DEPENDENCY_NOT_INSTALLABLE};
          errorLists->emplace_back(err);
          resolved = false;
        }
      }
    } else {
      auto err = PackageError{pkg, dep, Package{{}, {}, {}, {}},
                              PackageError::ErrorType::DEPENDENCY_NOT_FOUND};
      errorLists->emplace_back(err);
      resolved = false;
    }
  }
};