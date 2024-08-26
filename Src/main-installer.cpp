/**
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：main
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include <getopt.h>

#include "utils.h"
#include "pkg/installer.h"

static void showHelpMessage();
static void showVersionInfo();

int main_installer(int argc, char** argv) {
  std::string pkg = {};
  std::string target = {};
  std::string file = {};
  bool volinfo = false, dominfo = false, pkginfo = false;
  bool verboseR = false;

  struct option installer_options[] = {
      { "pkg", required_argument, 0, 0},
      { "package", required_argument, 0, 0},
      { "volinfo", no_argument, 0, 0},
      { "dominfo", no_argument, 0, 0},
      { "pkginfo", no_argument, 0, 0},
      { "allowUntrusted", no_argument, 0, 0},
      { "vers", no_argument, 0, 0},
      { "target", required_argument, 0, 0},
      { "file", required_argument, 0, 0},
      { "help", no_argument, 0, 0},
      { "verboseR", no_argument, 0, 0},
      { nullptr, 0, 0, 0}
  };

  if (argc == 1)
    showHelpMessage();

  try {
    while (true) {
      int c, option_index;

      c = getopt_long_only(argc, argv, "", installer_options, &option_index);

      if (c != 0) break;

      switch (hash_(installer_options[option_index].name)) {
        case hash_compile_time("pkg"):
          break;
        case hash_compile_time("package"):
          if (!pkg.empty())
            throw std::runtime_error(
                "-pkg/-package options cannot be repeated");
          pkg = optarg;
          break;
        case hash_compile_time("volinfo"):
          volinfo = true;
          break;
        case hash_compile_time("dominfo"):
          dominfo = true;
          break;
        case hash_compile_time("pkginfo"):
          pkginfo = true;
          break;
        case hash_compile_time("allowUntrusted"):
          break;
        case hash_compile_time("vers"):
          showVersionInfo();
          break;
        case hash_compile_time("target"):
          target = optarg;
          break;
        case hash_compile_time("file"):
          file = optarg;
          break;
        case hash_compile_time("help"):
          showHelpMessage();
          break;
        case hash_compile_time("verboseR"):
          verboseR = true;
          break;
      }
    }

    if (optind < argc) showHelpMessage();

    if (pkg.empty())
      throw std::runtime_error("-package argument is required");
    if (int(volinfo) + int(dominfo) + int(pkginfo) > 1)
      throw std::runtime_error(
          "Cannot specify -volinfo, -dominfo or -pkginfo at the same time");

    if (volinfo) {
      // throw std::runtime_error("-volinfo is not implemented yet");
      // TODO
      std::cout << "/\n";
    } else if (dominfo) {
      // throw std::runtime_error("-dominfo is not implemented yet");
      std::cout << "LocalSystem\n";
    } else if (pkginfo) {
      throw std::runtime_error("-pkginfo is not implemented yet");
    } else if (!file.empty()) {
      throw std::runtime_error("-file is not implemented yet");
    } else {
      if (target.empty()) throw std::runtime_error("Must specify -target");

      Installer installer(pkg, target);

      // if (verboseR) installer.enableVerboseR();

      // installer.installPackage();
    }

    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

static void showHelpMessage() {
  std::cerr << "Usage: installer [-help] [-dominfo] [-volinfo] [-pkginfo] "
               "[-allowUntrusted]\n"
               "                 -pkg <pathToPackage>\n"
               "                 -target <[DomainKey|MountPoint]>\n";
  std::exit(1);
}

static void showVersionInfo() {
  std::cerr << "Darling installer version UNRELEASED\n";
  exit(0);
}