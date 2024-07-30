#include "pkm.h"

#include <algorithm>
#include <cctype>
#include <memory>

bool _isDigit(const std::string& str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

Package::Package(const Package& other) {
  name = other.name;
  version = other.version;
  dependencies = other.dependencies;
  status = other.status;
}

void PackageManager::addPackage(const Package& pkg) {
  packages.emplace(pkg.name, pkg);
}

bool PackageManager::checkDependencies(
    const Package& pkg, std::shared_ptr<std::vector<Package>> pkgInstallList) {
  if (!pkgInstallList) {
    pkgInstallList = std::make_shared<std::vector<Package>>();
  }

  bool resolved = true;
  auto check_deps = [&, this, pkgInstallList](bool& resolved) -> void {
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

VersionNumberPart::VersionNumberPart(std::string value) : value(value) {}

std::string VersionNumberPart::getValue() const { return value; }

bool VersionNumberPart::operator<(const VersionNumberPart& other) const {
  if (_isDigit(value) && _isDigit(other.value)) {
    return std::stoi(value) < std::stoi(other.value);
  } else if (_isDigit(value) && !_isDigit(other.value)) {
    // Charactors is always bigger than numbers
    return true;
  } else if (!_isDigit(value) && _isDigit(other.value)) {
    return false;
  } else {
    // Now, both are characters

    // Check special conditions first
    // i.e. . < ~ < +
    // Note that: 1.0.x > 1.0+x > 1.0~x
    if (value == "." && other.value != ".") {
      // . is bigger as it indicats that
      // Current version string is longer
      return false;
    } else if (value == "~" && other.value != "~") {
      // ~ is always smaller than any other version
      return true;
    } else if (value == "+" && other.value != "+") {
      // + is always bigger than any other version
      return false;
    }

    if (other.value == "." && value != ".") {
      return true;
    } else if (other.value == "~" && value != "~") {
      return false;
    } else if (other.value == "+" && value != "+") {
      return true;
    }

    // Otherwise, we can compare the two strings directly
    // In C++, strings are compared by ASCII code.
    // Bigger ASCII code is bigger, same as the rules.
    return value < other.value;
  }
  return false;
}

bool VersionNumberPart::operator>(const VersionNumberPart& other) const {
  if (_isDigit(value) && _isDigit(other.value)) {
    return std::stoi(value) > std::stoi(other.value);
  } else if (_isDigit(value) && !_isDigit(other.value)) {
    // Charactors is always bigger than numbers
    return false;
  } else if (!_isDigit(value) && _isDigit(other.value)) {
    return true;
  } else {
    // Now, both are characters

    // Check special conditions first
    // i.e. . < ~ < +
    // Note that: 1.0.x > 1.0+x > 1.0~x
    if (value == "." && other.value != ".") {
      // . is bigger as it indicats that
      // Current version string is longer
      return true;
    } else if (value == "~" && other.value != "~") {
      // ~ is always smaller than any other version
      return false;
    } else if (value == "+" && other.value != "+") {
      // + is always bigger than any other version
      return true;
    }

    if (other.value == "." && value != ".") {
      return false;
    } else if (other.value == "~" && value != "~") {
      return true;
    } else if (other.value == "+" && value != "+") {
      return false;
    }

    // Otherwise, we can compare the two strings directly
    // In C++, strings are compared by ASCII code.
    // Bigger ASCII code is bigger, same as the rules.
    return value > other.value;
  }
  return false;
}

bool VersionNumberPart::operator==(const VersionNumberPart& other) const {
  return value == other.value;
}

/**
 * Compare package version for pkg1 and pkg2
 * Result: pkg1 is xxx than pkg2
 */
VersionCompareResult comparePkgVersion(const std::string& pkg1,
                                       const std::string& pkg2) {

  // If they are the same, return EQUAL
  if (pkg1 == pkg2) {
    return VersionCompareResult::EQUAL;
  }

  // Split two version string
  auto split_version =
      [](const std::string& version) -> std::vector<VersionNumberPart> {
    std::vector<VersionNumberPart> result;
    std::string item;

    // Write into temp string untill we reached any non-numeric character
    for (auto& c : version) {
      if (isdigit(c)) {
        item += c;
      } else {
        // Write to result list
        if (item.length() > 0) result.emplace_back(VersionNumberPart(item));
        item.clear();

        // Wirite the character to result list
        item += c;
        result.emplace_back(VersionNumberPart(item));
        item.clear();
      }
    }

    // Write the last item
    result.emplace_back(VersionNumberPart(item));
    return result;
  };

  auto pkg1_version = split_version(pkg1);
  auto pkg2_version = split_version(pkg2);

  // Get the longest version string for iteration use
  auto max_length = pkg1_version.size() > pkg2_version.size()
                        ? pkg1_version.size()
                        : pkg2_version.size();
  auto pkg1_version_length = pkg1_version.size();
  auto pkg2_version_length = pkg2_version.size();

  for (auto i = 0; i < max_length; i++) {
    // If we reached one of the string's end,
    // and still cannot identify which one is bigger,
    // The longest one consider the new/bigger one.
    if (i >= pkg1_version_length) {
      if (pkg2_version[i] == VersionNumberPart("~")) {
        // This means that, pkg1 and pkg2 has same version string 
        // brfore index 1. However, if the longer one is ending
        // with ~, it is always smaller than the shorter one that 
        // dose not have ~
        return VersionCompareResult::NEWER;
      } else {
        return VersionCompareResult::OLDER;
      }
    } else if (i >= pkg2_version_length) {
      if (pkg1_version[i] == VersionNumberPart("~")) {
        return VersionCompareResult::OLDER;
      } else {
        return VersionCompareResult::NEWER;
      }
    }

    // Compare two version number part
    if (pkg1_version[i] < pkg2_version[i]) {
      return VersionCompareResult::OLDER;
    } else if (pkg1_version[i] > pkg2_version[i]) {
      return VersionCompareResult::NEWER;
    }
  }

  return VersionCompareResult::UNKNOWN;
}
