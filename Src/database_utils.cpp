/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: database_utils.cpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: class and functions used for database
 * Others:
 * Function List:
 * History:
 *   Elysia 24/07/31 0.0.1 Create this file.
 *************************************************/

#include "database_utils.hpp"

#include <memory>

DatabaseUtils::DatabaseUtils(std::string dbPath) {
  // Create or open database
  m_dbPath = dbPath;
  m_db = std::make_shared<SQLite::Database>(
      m_dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}