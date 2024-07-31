/*************************************************
 * Copyright (C), 2024, Lingmo Community
 * File name: package_utils.cpp
 * Author: Elysia
 * Version: 0.0.1
 * Date: 24/07/31
 * Description: Definitions of package related functions.
 * Others: 
 * Function List: 
 * History: 
 *   Elysia 24/07/31 0.0.1 Create this file. 
 *************************************************/

 #include "package_utils.hpp"

 Package::Package(const Package& other) {
  name = other.name;
  version = other.version;
  dependencies = other.dependencies;
  status = other.status;
}