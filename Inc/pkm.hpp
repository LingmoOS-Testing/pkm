#ifndef __PKM_H__
#define __PKM_H__

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "package_utils.hpp"


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