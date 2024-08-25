/**
 *  SPDX-License-Identifier: MulanPSL-2.0
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：main entry of the program.
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */

#include <cstring>
#include <filesystem>
#include <iostream>

#include "utils.h"

std::string getExecutableName(char* argv0);

// Entry for functionalities
int main_lsbom(int argc, char** argv);
int main_installer(int argc, char** argv);
int main_uninstaller(int argc, char** argv);
int main_pkgutil(int argc, char** argv);

int main(int argc, char** argv) {
  if (!argc) {
    std::cerr << "No argument found! Existing.";
    return -1;
  }

  auto currentProgname = getExecutableName(argv[0]);
  if (currentProgname.empty()) {
    std::cerr << "No valid executable name found! Existing.";
    return -1;
  }

  // Jump to real main function
  switch (hash_(currentProgname.c_str())) {
    case hash_compile_time("lsbom"):
      // return main_lsbom(argc, argv);
      std::cout << "Not implemented yet!";
      return 0;
    case hash_compile_time("uninstaller"):
      // return main_uninstaller(argc, argv);
      std::cout << "Not implemented yet!";
      return 0;
    case hash_compile_time("pkgutil"):
      // return main_pkgutil(argc, argv);
      std::cout << "Not implemented yet!";
      return 0;
    case hash_compile_time("installer"):
      return main_installer(argc, argv);
    default:
      std::cout << "Not implemented yet!";
      return 0;
  }
}

std::string getExecutableName(char* argv0) {
  namespace fs = std::filesystem;
  fs::path exePath = fs::absolute(fs::path(argv0));
  if (fs::exists(exePath)) {
    return exePath.filename();
  } else {
    return {};
  }
}