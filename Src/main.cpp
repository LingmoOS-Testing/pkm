#include <iostream>

#include "pkm.hpp"
int main() {
  PackageManager manager;

  // 子包定义
  Package SubDep1("SubDep1", "2.0.0", {}, PackageStatus::UNINSTALLED);
  Package SubDep2("SubDep2", "1.5.0", {}, PackageStatus::UNINSTALLED);
  Package SubDep3("SubDep3", "3.0.0", {}, PackageStatus::UNINSTALLED);

  // 一级子包定义
  Package Dep1(
      "Dep1", "1.0.0-1",
      {{"SubDep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "2.0.0~test"},
       {"SubDep2", VersionCompareIdentifier::SMALLER, "2.0.0"}},
      PackageStatus::UNINSTALLED);

  Package Dep2("Dep2", "1.0.0",
               {{"SubDep3", VersionCompareIdentifier::EQUAL, "3.0.0"}},
               PackageStatus::UNINSTALLED);

  // 添加到包管理器
  manager.addPackage(SubDep1);
  manager.addPackage(SubDep2);
  manager.addPackage(SubDep3);
  manager.addPackage(Dep1);
  manager.addPackage(Dep2);

  // 待安装的主包
  Package MainApp(
      "MainApp", "1.0",
      {{"Dep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "3.0.0"},
       {"Dep10", VersionCompareIdentifier::EQUAL, "1.0.0"}},
      PackageStatus::TOINSTALL);

  auto pkgInstList = std::make_shared<std::vector<Package>>();
  auto errorList = std::make_shared<std::vector<PackageError>>();

  if (manager.checkDependencies(MainApp, pkgInstList, errorList)) {
    std::cout << "Successfully resolved deps." << std::endl;
    std::cout << "Need to install following package(s):" << std::endl;
    for (auto const &p : *pkgInstList) {
      std::cout << "  " << p.name << " " << p.version << std::endl;
    }

  } else {
    std::cout << "Unable to resolve deps" << std::endl;

    for (auto const &e : *errorList) {
      switch (e.errorType) {
        case PackageError::ErrorType::DEPENDENCY_NOT_FOUND:
          std::cout << "  " << e.currentPackage.name << " "
                    << e.currentPackage.version << " depends on "
                    << std::get<0>(e.wantedDependency) << " "
                    << std::get<2>(e.wantedDependency) << " "
                    << "which is not found." << std::endl;
          break;
        case PackageError::ErrorType::DEPENDENCY_NOT_MATCH:
          std::cout << "  " << e.currentPackage.name << " "
                    << e.currentPackage.version << " depends on "
                    << std::get<0>(e.wantedDependency) << " "
                    << std::get<2>(e.wantedDependency) << " "
                    << "but " << e.currentDependency.name << " "
                    << e.currentDependency.version << " "
                    << "is to be installed." << std::endl;
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