/**
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：installer
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */

#include "pkg/installer.h"

#include <sys/stat.h>

#include <climits>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

Installer::Installer(std::string pkg, std::string target)
    : m_pkg{std::move(pkg)}, m_target{std::move(target)} {
  struct stat st{};
  char origCwd[PATH_MAX];

  std::string target_ = m_target;
  // DomainKey
  if (target_ == "LocalSystem")
    target_ = "/";
  else if (target_[0] != '/')
    throw std::runtime_error("Invalid target path");

  if (::stat(target_.c_str(), &st) != 0)
    throw std::runtime_error("Invalid target path");

  if (!S_ISDIR(st.st_mode))
    throw std::runtime_error("Invalid target path; not a directory");
}

void Installer::installPackage() {}
