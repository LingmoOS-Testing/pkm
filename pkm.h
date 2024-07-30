#ifndef __PKM_H__
#define __PKM_H__

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <memory>

enum class PackageStatus {
    UNINSTALLED = 0,
    INSTALLED,
    TOINSTALL
};

struct Package {
  std::string name;
  std::string version;
  std::vector<std::tuple<std::string, std::string>> dependencies;

  PackageStatus status;

  Package(const Package &other);

  inline Package(std::string name, std::string version, 
                 std::vector<std::tuple<std::string, std::string>> dependencies, PackageStatus status = PackageStatus::UNINSTALLED)
      : name(name), version(version), dependencies(dependencies), status(status) {}
};

class PackageManager {
 public:
  void addPackage(const Package &pkg);
  bool checkDependencies(const Package &pkg, std::shared_ptr<std::vector<Package>> pkgInstallList = nullptr);

 private:
  std::map<std::string, Package> packages;
};

#endif  //__PKM_H__