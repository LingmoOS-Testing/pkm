/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：test_normal_deps
 *  Author：Elysia
 *  Date: 25-5-29
 *  Modify Record:
 */
#include <gtest/gtest.h>

#include "pkm.hpp"
#include "version_utils.hpp"

// Test dependency resolve
TEST(cpkm_test_dependency, BasicDpendencyResolve) {
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
  // Expect equal.
  EXPECT_TRUE(manager.checkDependencies(MainApp, pkgInstList, errorList));
}

// Test package version compare
TEST(cpkm_test_dependency, PackageVersionCompare) {
  // Test simple version number
  EXPECT_EQ(comparePkgVersion("1.0.0", "1.0.0"), VersionCompareIdentifier::EQUAL);
  EXPECT_EQ(comparePkgVersion("1.0.0", "1.0.1"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("1.0.0", "1.1.0"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("1.0.0", "2.0.0"), VersionCompareIdentifier::SMALLER);

  // Test version numbers that contains special characters
  EXPECT_EQ(comparePkgVersion("1.0.0-alpha", "1.0.0-alpha"), VersionCompareIdentifier::EQUAL);
  EXPECT_EQ(comparePkgVersion("1.0.0-alpha", "1.0.0-beta"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("1.0.0-alpha", "1.0.0-alpha.1"), VersionCompareIdentifier::SMALLER);

  // contain ~ . +
  EXPECT_EQ(comparePkgVersion("1.0.0+1", "1.0.0"), VersionCompareIdentifier::GREATOR);
  EXPECT_EQ(comparePkgVersion("1.0.0~1", "1.0.0"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("1.0.0-1", "1.0.0"), VersionCompareIdentifier::GREATOR);
  EXPECT_EQ(comparePkgVersion("1.0.0-1", "1.0.0~1"), VersionCompareIdentifier::GREATOR);

  // Test other version number
  EXPECT_EQ(comparePkgVersion("3", "3.14"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("1.2.1", "a.1"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("a.b.c", "a.b.d"), VersionCompareIdentifier::SMALLER);
  EXPECT_EQ(comparePkgVersion("3-1", "3+1"), VersionCompareIdentifier::SMALLER);
}