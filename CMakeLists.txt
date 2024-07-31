cmake_minimum_required(VERSION 3.15.0)
project(pkm VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


# 添加第三方依赖包
set(SQLITECPP_RUN_CPPLINT OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SQLiteCpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp)

add_executable(pkm ${SRC_FILES})
target_link_libraries(pkm
  SQLiteCpp
  sqlite3
  pthread
  dl
)
