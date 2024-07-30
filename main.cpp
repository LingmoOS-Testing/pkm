#include "pkm.h"

int main() {
  PackageManager manager;

  Package Dep1("Dep1", "1.0", {}, PackageStatus::UNINSTALLED);
  Package Dep2("Dep2", "1.0", {}, PackageStatus::UNINSTALLED);
  manager.addPackage(Dep1);
  manager.addPackage(Dep2);

  Package testPackage("TestApp", "1.0", {{"Dep1", "2.0.0"}, {"Dep2", "1.0.0"}}, PackageStatus::TOINSTALL);

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
}