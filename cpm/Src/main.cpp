#include <iostream>

#include "log.h"
#include "pkm.hpp"

#include "database_utils.hpp"

#include <cstdint>

int main1() {
  auto db = DatabaseUtils();
  // db.m_initDatabase();
  db.getPackage("test");
  return 0;
}

int main() {
#ifdef CPKM_ENABLE_DEBUG
  log_set_level(LOG_DEBUG);
#endif

  PackageManager manager;

  // 子包定义
  Package SubDep1("SubDep1", "2.0.0", {}, PackageStatus::UNINSTALLED);
  Package SubDep2("SubDep2", "1.5.0", {}, PackageStatus::UNINSTALLED);
  Package SubDep3("SubDep3", "3.0.0", {}, PackageStatus::UNINSTALLED);
  Package SubDep3_old("SubDep3", "2.0.0", {}, PackageStatus::UNINSTALLED);

  // 一级子包定义
  Package Dep1(
      "Dep1", "1.0.0-1",
      {{"SubDep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "2.0.0~test"},
       {"SubDep2", VersionCompareIdentifier::SMALLER, "2.0.0"}},
      PackageStatus::UNINSTALLED);

  Package Dep2("Dep2", "1.0.0",
               {{"SubDep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "1.0.0"},
               {"Dep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "1.0.0"}},
               PackageStatus::UNINSTALLED);

  // 添加到包管理器
  manager.addPackage(SubDep1);
  manager.addPackage(SubDep2);
  // manager.addPackage(SubDep3);
  manager.addlocalInstalledPackage(SubDep3_old);
  manager.addPackage(Dep1);
  manager.addPackage(Dep2);

  // 待安装的主包
  Package MainApp(
      "MainApp", "1.0",
      {{"Dep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "1.0.0"},
       {"Dep2", VersionCompareIdentifier::EQUAL, "1.0.0"}},
      PackageStatus::TOINSTALL);

  auto pkgInstList = std::make_shared<std::list<Package>>();
  auto errorList = std::make_shared<std::list<PackageError>>();

  if (manager.checkDependencies(MainApp, pkgInstList, errorList)) {
    std::cout << "Successfully resolved deps." << std::endl;
    std::cout << "Need to install following package(s):" << std::endl;
    for (const auto &it : *pkgInstList) {
      std::cout << "  " << it.name << " " << it.version << std::endl;
    }
  } else {
    std::cout << "Unable to resolve deps" << std::endl;
    
    for (auto i = errorList->cend(); i != errorList->cbegin(); i--) {
      switch (i->errorType) {
        case PackageError::ErrorType::DEPENDENCY_NOT_FOUND:
          std::cout << "  " << i->currentPackage.name << " "
                    << i->currentPackage.version << " depends on "
                    << i->wantedDependency.name << " "
                    << i->wantedDependency.version << " "
                    << "that is not found." << std::endl;
          break;
        case PackageError::ErrorType::DEPENDENCY_NOT_MATCH:
          std::cout << "  " << i->currentPackage.name << " "
                    << i->currentPackage.version << " depends on "
                    << i->wantedDependency.name << " "
                    << i->wantedDependency.version << " "
                    << "but only " << i->currentDependency.name << " "
                    << i->currentDependency.version << " "
                    << "is installable." << std::endl;
          break;
        case PackageError::ErrorType::DEPENDENCY_NOT_INSTALLABLE:
          std::cout << "  " << i->currentPackage.name << " "
                    << i->currentPackage.version << " depends on "
                    << i->wantedDependency.name << " "
                    << i->wantedDependency.version << " "
                    << "but " << i->currentDependency.version << " "
                    << "is planned to be installed by another package."<< std::endl;
          break;
        default:
          break;
      }
    }
  }

  return 0;

  // auto result1 = comparePkgVersion("1.0.0", "1.0.0");
  // auto result2 = comparePkgVersion("1.0.0", "1.0.1");
  // auto result3 = comparePkgVersion("1.0.0", "1.1.0");
  // auto result4 = comparePkgVersion("1.0.0", "2.0.0");

  // // Test version numbers that contains special characters
  // auto result5 = comparePkgVersion("1.0.0-alpha", "1.0.0-alpha");
  // auto result6 = comparePkgVersion("1.0.0-alpha", "1.0.0-beta");
  // auto result7 = comparePkgVersion("1.0.0-alpha", "1.0.0-alpha.1");
  // // contain ~ . +
  // auto result8 = comparePkgVersion("1.0.0+1", "1.0.0");
  // auto result9 = comparePkgVersion("1.0.0~1", "1.0.0");
  // auto result10 = comparePkgVersion("1.0.0-1", "1.0.0");
  // auto result11 = comparePkgVersion("1.0.0-1", "1.0.0~1");

  // // Test other version number
  // auto result12 = comparePkgVersion("3", "3.14");
  // auto result13 = comparePkgVersion("1.2.1", "a.1");
  // auto result14 = comparePkgVersion("a.b.c", "a.b.d");
  // auto result15 = comparePkgVersion("3-1", "3+1");
  // return 0;
}