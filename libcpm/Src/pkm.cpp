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

#include <algorithm>
#include <cctype>
#include <cfloat>
#include <cstdio>
#include <memory>

#include "log.h"
#include "package_utils.hpp"

void PackageManager::addPackage(const Package& pkg) {
  packageCacheList.emplace(pkg.name, pkg);
}

void PackageManager::addlocalInstalledPackage(const Package& pkg) {
  packageInstalledList.emplace(pkg.name, pkg);
}

bool PackageManager::checkDependencies(
    const Package& pkg, std::shared_ptr<std::list<Package>> pkgInstallList,
    std::shared_ptr<std::list<PackageError>> errorLists) {
  log_debug("Checking package: %s", pkg.name.c_str());
  log_debug("It has following dependencies:");

  packageUnderChecking = &pkg;

  for (const Dependency& p : pkg.dependencies) {
    log_debug("\t%s %s", p.name.c_str(), p.version.c_str());
  }

  if (!pkgInstallList) {
    pkgInstallList = std::make_shared<std::list<Package>>();
  }

  if (!errorLists) {
    errorLists = std::make_shared<std::list<PackageError>>();
  }

  bool resolved = true;

  // Check current package status
  switch (pkg.status) {
    case PackageStatus::INSTALLED:
      // ToDo: Checking recursively to test dependency, like wether the deps is
      // missing or we need to update them
      log_debug("Package \"%s\" installed, checking its deps status.",
                pkg.name.c_str());
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);
      break;

    case PackageStatus::UNINSTALLED:
      // If current dep is not installed,
      // we may want to test if it can be installed <_<
      log_debug("Package \"%s\" is not installed, checking if it can be inst.",
                pkg.name.c_str());
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);

      if (resolved) {
        log_debug("\tResolved, add %s into install list", pkg.name.c_str());
        pkgInstallList->push_back(pkg);
      }
      break;

    case PackageStatus::TOINSTALL:
      log_debug("Requested to install Package \"%s\".", pkg.name.c_str());
      m_checkPackageStatus(resolved, pkg, pkgInstallList, errorLists);

      if (resolved) {
        log_debug("\tResolved, add %s into install list", pkg.name.c_str());
        pkgInstallList->push_back(pkg);
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
    std::shared_ptr<std::list<Package>> pkgInstallList,
    std::shared_ptr<std::list<PackageError>> errorLists) {
  // Iterate through every required dependencies
  for (const auto& dep : pkg.dependencies) {
    // Prevent recursive searching
    if (pkg.name == dep.name) {
      log_error("The required package: %s is being requied recursively!", dep.name.c_str());
      auto err =
          PackageError{pkg, dep, pkg,
                        PackageError::ErrorType::DEPENDENCY_CIRCULAR_REFERENCE};
      errorLists->emplace_back(err);
      resolved = false;
      continue;
    }
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
    auto it = std::find_if(pkgInstallList->cbegin(), pkgInstallList->cend(),
                           [&dep](const Package& pkg) {
                             if (pkg.name == dep.name)
                               return true;
                             else
                               return false;
                           });
    if (it->name == dep.name) {
      if (auto dep_in_inst_list = Package(*it);
          m_pkgVersionChecker(dep_in_inst_list.version, dep.version, dep.compare_id)) {
        log_debug("\t\tPackage %s already resolved", dep.name.c_str());
        continue;  // Have required version install, continue.
      } else {
        // The package is planned to be installed,
        // but the current needed version is not available or conflict
        // with the planned one
        auto err =
            PackageError{pkg, dep, dep_in_inst_list,
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
      if (auto dep_in_cache_list = packageCacheList.at(dep.name);
          !m_pkgVersionChecker(dep_in_cache_list.version, dep.version, dep.compare_id)) {
        // If the package is not install and we don't have the required
        // version Raise error
        auto err = PackageError{pkg, dep, dep_in_cache_list,
                                PackageError::ErrorType::DEPENDENCY_NOT_MATCH};
        errorLists->emplace_back(err);
        resolved = false;
        continue;
      } else {
        // Otherwise, we need to check the dependency of this package
        // recursively
        if (this->checkDependencies(dep_in_cache_list, pkgInstallList, errorLists)) {
          // If the sub-dependency is resolved, we can continue
          packageUnderChecking = &pkg;
          continue;
        } else {
          auto err =
              PackageError{pkg, dep, dep_in_cache_list,
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