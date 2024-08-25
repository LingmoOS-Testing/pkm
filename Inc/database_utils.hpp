/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: database_utils.hpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: class and functions used for database
 * Others:
 * Function List:
 * History:
 *   Elysia 24/07/31 0.0.1 Create this file.
 *************************************************/

#ifndef __DATABASE_UTILS_HPP__
#define __DATABASE_UTILS_HPP__

#define DB_PATH "/cygdrive/e/Projects/LingmoOS/pkm/pkm.db"

#include <SQLiteCpp/SQLiteCpp.h>

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "package_utils.hpp"

class DatabaseUtils {
 private:
  std::shared_ptr<SQLite::Database> m_db;

  std::string m_dbPath;

 public:
  DatabaseUtils(std::string dbPath = DB_PATH);

  bool m_initDatabase();

  bool getPackage(const std::string& pkg);

  std::vector<Dependency> getDependencies(const ::int64_t& packageId);
};

#endif  // __DATABASE_UTILS_HPP__