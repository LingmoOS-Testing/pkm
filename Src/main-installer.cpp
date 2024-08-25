/**
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：main
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */

#include <cstdlib>
#include <iostream>

static void showHelpMessage();

int main_installer(int argc, char** argv) {
  showHelpMessage();
  return 0;
}

static void showHelpMessage() {
  std::cerr << "Usage: installer [-help] [-dominfo] [-volinfo] [-pkginfo] "
               "[-allowUntrusted]\n"
               "                 -pkg <pathToPackage>\n"
               "                 -target <[DomainKey|MountPoint]>\n";
  std::exit(1);
}