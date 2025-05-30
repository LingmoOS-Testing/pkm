/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: package_utils.hpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: Definitions of package related functions.
 * Others:
 * Function List:
 * History:
 *   Elysia 24/07/31 0.0.1 Create this file.
 *************************************************/

#ifndef __PACKAGE_UTILS_HPP__
#define __PACKAGE_UTILS_HPP__

#include <string>
#include <tuple>
#include <vector>

#include "version_utils.hpp"

/**
 * @Name: enum class PackageStatus
 * @Description: A enum class to describe the install status of a package.
 * @Others:
 */
enum class PackageStatus { UNINSTALLED = 0, INSTALLED, TOINSTALL };

/**
 * @Name: enum class PackageInstallType
 * @Description: AUTO means package is automatically installed as a dependency.
 * @Others:
 */
enum class PackageInstallType { AUTO = 0, MANUAL };

/**
 * @Name struct Dependency
 * @Description: Dependency structure
 */
struct Dependency {
  std::string name;

  VersionCompareIdentifier compare_id;

  std::string version;
};

/**
 * @Name: struct Package
 * @Description: A class to store and process a package's information.
 * @Others:
 */
struct Package {
  std::string name;
  std::string version;
  std::vector<Dependency>
      dependencies;  // name, compare method, version requirement

  PackageStatus status;

  Package(const Package &other);

  Package(std::string name, std::string version,
          std::vector<Dependency> dependencies,
          PackageStatus status = PackageStatus::UNINSTALLED);
};

class PackageError {
 public:
  enum class ErrorType {
    UNKNOWN = 0,
    VERSION_NOT_MATCH,
    DEPENDENCY_NOT_MATCH,
    DEPENDENCY_NOT_FOUND,
    DEPENDENCY_CIRCULAR_REFERENCE,
    DEPENDENCY_NOT_INSTALLED,
    DEPENDENCY_NOT_INSTALLABLE,
    DEPENDENCY_NOT_UNINSTALLABLE,
    DEPENDENCY_NOT_UPDATABLE,
  };

  Package currentPackage;

  Dependency wantedDependency;

  Package currentDependency;

  ErrorType errorType;

  inline PackageError(Package currentPackage, Dependency wantedPackage,
                      Package currentDependency, ErrorType errorType)
      : currentPackage(currentPackage),
        wantedDependency(wantedPackage),
        currentDependency(currentDependency),
        errorType(errorType) {}
};

#endif  // End __PACKAGE_UTILS_HPP__
