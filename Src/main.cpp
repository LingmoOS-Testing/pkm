#include <iostream>

#include "pkm.hpp"
int main() {
  PackageManager manager;

  Package Dep1("Dep1", "1.0.0+1", {}, PackageStatus::UNINSTALLED);
  Package Dep2("Dep2", "1.0.0", {}, PackageStatus::UNINSTALLED);
  manager.addPackage(Dep1);
  manager.addPackage(Dep2);

  Package testPackage("TestApp", "1.0",
                      {{"Dep1", VersionCompareIdentifier::GREATOR_OR_EQUAL, "1.0.0"},
                       {"Dep2", VersionCompareIdentifier::EQUAL, "1.0.0"}},
                      PackageStatus::TOINSTALL);

  auto pkgInstList = std::make_shared<std::vector<Package>>();
  if (manager.checkDependencies(testPackage, pkgInstList)) {
    std::cout << "Successfully resolved deps." << std::endl;
    std::cout << "Need to install following package(s):" << std::endl;
    for (auto const &p : *pkgInstList) {
      std::cout << "  " << p.name << " " << p.version << std::endl;
    }

  } else {
    std::cout << "Unable to resolve deps" << std::endl;
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