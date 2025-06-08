/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: pkm.hpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: Package Manager's Class definition
 * Others:
 * Function List:
 * History:
 *   Elysia 24/07/31 0.0.1 Create this file.
 *************************************************/

#ifndef __PKM_H__
#define __PKM_H__

#include <map>
#include <memory>
#include <string>
#include <list>

#include "package_utils.hpp"

class PackageManager {
 public:
  void addPackage(const Package& pkg);
  //For test only
  void addlocalInstalledPackage(const Package& pkg);
  
  bool checkDependencies(
      const Package& pkg,
      std::shared_ptr<std::list<Package>> pkgInstallList = nullptr,
      std::shared_ptr<std::list<PackageError>> errorLists = nullptr);

 private:
  std::map<std::string, Package> packageCacheList;

  std::map<std::string, Package> packageInstalledList;

  bool m_pkgVersionChecker(const std::string& pkg1, const std::string& pkg2,
                           const VersionCompareIdentifier& compare_identifier);

  void m_checkPackageStatus(
      bool& resolved, const Package& pkg,
      std::shared_ptr<std::list<Package>> pkgInstallList,
      std::shared_ptr<std::list<PackageError>> errorLists);
};

#endif  //__PKM_H__