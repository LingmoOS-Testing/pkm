#ifndef __PKM_H__
#define __PKM_H__

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

enum class PackageStatus { UNINSTALLED = 0, INSTALLED, TOINSTALL };

struct Package {
  std::string name;
  std::string version;
  std::vector<std::tuple<std::string, std::string>> dependencies;

  PackageStatus status;

  Package(const Package &other);

  inline Package(std::string name, std::string version,
                 std::vector<std::tuple<std::string, std::string>> dependencies,
                 PackageStatus status = PackageStatus::UNINSTALLED)
      : name(name),
        version(version),
        dependencies(dependencies),
        status(status) {}
};

enum class VersionCompareResult { EQUAL = 0, OLDER, NEWER, UNKNOWN };

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

 public:
  bool operator<(const VersionNumberPart &other) const;

  bool operator>(const VersionNumberPart &other) const;

  bool operator==(const VersionNumberPart &other) const;
};

VersionCompareResult comparePkgVersion(const std::string &pkg1,
                                       const std::string &pkg2);

class PackageManager {
 public:
  void addPackage(const Package &pkg);
  bool checkDependencies(
      const Package &pkg,
      std::shared_ptr<std::vector<Package>> pkgInstallList = nullptr);

 private:
  std::map<std::string, Package> packages;
};

#endif  //__PKM_H__