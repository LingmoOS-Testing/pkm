/**
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：installer
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */
#ifndef INSTALLER_INC_PKG_INSTALLER_H_
#define INSTALLER_INC_PKG_INSTALLER_H_

#include <string>

class Installer {
 public:
  Installer(std::string  pkg, std::string  target);

  void installPackage();
  void enableVerboseR() { m_verboseR = true; }

 private:
  bool m_verboseR = false;
  std::string m_pkg;
  std::string m_target;

};
#endif  // INSTALLER_INC_PKG_INSTALLER_H_
