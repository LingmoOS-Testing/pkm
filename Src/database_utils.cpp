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

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "SQLiteCpp/Transaction.h"

DatabaseUtils::DatabaseUtils(std::string dbPath) {
  // Create or open database
  m_dbPath = dbPath;
  m_db = std::make_shared<SQLite::Database>(
      m_dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}

bool DatabaseUtils::m_initDatabase() {
  try {
    m_db->exec(R"(DROP TABLE IF EXISTS "main"."packages")");
    m_db->exec(R"(DROP TABLE IF EXISTS "main"."dependencies")");

    SQLite::Transaction transaction(*m_db);

    m_db->exec(R"(CREATE TABLE "packages" (
              "id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
              "name"  TEXT NOT NULL COLLATE BINARY ,
              "version"  TEXT NOT NULL,
              "install_type"  INTEGER NOT NULL
              );)");

    m_db->exec(R"(CREATE TABLE dependencies (
              id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
              package_id INTEGER NOT NULL,
              dependency_id INTEGER NOT NULL,
              version VARCHAR(255) NOT NULL,
              version_requirement INTEGER(255) NOT NULL,
              FOREIGN KEY (package_id) REFERENCES packages(id) ON DELETE CASCADE,
              FOREIGN KEY (dependency_id) REFERENCES packages(id) ON DELETE CASCADE
              );)");

    // Commit transaction
    transaction.commit();
  } catch (std::exception& e) {
    std::cout << "Database exception: " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool DatabaseUtils::getPackage(const std::string& pkg) {
  try {
    SQLite::Statement query(*m_db,
                            R"(SELECT * FROM main.packages WHERE name = ?)");

    query.bind(1, pkg);

    // We only need and normally can only have one package search result
    if (query.executeStep()) {
      ::int64_t id = query.getColumn(0);
      std::string name = query.getColumn(1);
      std::string version = query.getColumn(2);
      ::int64_t version_requirement = query.getColumn(3);

      // Then, we need to get its dependencies.
    } else {
      return false;
    }

  } catch (std::exception& e) {
    std::cout << "Database exception: " << e.what() << std::endl;
    return false;
  }
  return true;
}

std::vector<Dependency> DatabaseUtils::getDependencies(
    const int64_t& packageId) {
  try {
    SQLite::Statement query(
        *m_db, R"(SELECT * FROM main.dependencies WHERE package_id = ?)");

    query.bind(1, packageId);

    auto result = std::vector<Dependency>();

    while (query.executeStep()) {
      ::int64_t id = query.getColumn(0);
      ::int64_t package_id = query.getColumn(1);
      ::int64_t dependency_id = query.getColumn(2);
      std::string min_version = query.getColumn(3);
      std::string max_version = query.getColumn(4);

      std::string dependency_name;

      SQLite::Statement query_name(
          *m_db, R"(SELECT * FROM main.packages WHERE name = ?)");
      query_name.bind(1, dependency_id);

      if (query.executeStep()) {
        dependency_name = (std::string)query.getColumn(1);
      } else {
        throw std::exception();
      }

      //result.emplace_back({dependency_name, });
    }

  } catch (std::exception& e) {
    std::cout << "Database exception: " << e.what() << std::endl;
    return {};
  }
  return {};
}

