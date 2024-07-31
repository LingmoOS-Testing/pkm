/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: version_utils.hpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: Definitions of version helpers
 * Others: /
 * Function List:
 * History:
 *   Elysia 24/07/31 0.0.1 Create this file.
 *************************************************/

#ifndef __VERSION_UTILS_HPP__
#define __VERSION_UTILS_HPP__

#include <algorithm>
#include <string>

/**
 * @Name: enum class VersionCompareIdentifier
 * @Description: A enum class to describe the install status of a package.
 * @Others:
 */
enum class VersionCompareIdentifier {
  EQUAL = 0,
  SMALLER,
  GREATOR,
  UNKNOWN,
  GREATOR_OR_EQUAL,
  SMALLER_OR_EQUAL
};

/**
 * @Name: comparePkgVersion
 * @Description: Compare two packages' version string.
 *
 *   This function compares pkg1's version string with pkg2's version string.
 *   The output matchs this formula:
 *   pkg1's version is <result> than pkg2's version
 * @Calls:
 * @Called By:
 * @Input:
 *   1. pkg1: <const std::string> The version string of pkg1
 *   2. pkg2: <const std::string> The version string of pkg2
 * @Return: <VersionCompareIdentifier> The result of comparison, as described
 * above.
 * @Others:
 */
VersionCompareIdentifier comparePkgVersion(const std::string &pkg1,
                                           const std::string &pkg2);
/**
 * @brief VersionNumberPart
 *
 * Helper class to compare version number parts
 */
class VersionNumberPart {
 public:
  VersionNumberPart(std::string value);

  std::string getValue() const;

 private:
  std::string value;

  inline bool _isDigit(const std::string &str) const {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
  }

 public:
  bool operator<(const VersionNumberPart &other) const;

  bool operator>(const VersionNumberPart &other) const;

  bool operator==(const VersionNumberPart &other) const;
};

#endif  // End __VERSION_UTILS_HPP__