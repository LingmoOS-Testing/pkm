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
 * @Name: struct Package
 * @Description: A class to store and process a package's information.
 * @Others: 
 */
struct Package {
  std::string name;
  std::string version;
  std::vector<std::tuple<std::string, VersionCompareIdentifier, std::string>>
      dependencies;

  PackageStatus status;

  Package(const Package &other);

  inline Package(std::string name, std::string version,
                 std::vector<std::tuple<std::string, VersionCompareIdentifier,
                                        std::string>>
                     dependencies,
                 PackageStatus status = PackageStatus::UNINSTALLED)
      : name(name),
        version(version),
        dependencies(dependencies),
        status(status) {}
};

#endif  // End __PACKAGE_UTILS_HPP__
