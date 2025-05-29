cmake_minimum_required(VERSION 3.15.0)
project(cpkm VERSION 0.1.0 LANGUAGES C CXX)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(PKM_BUILD_TEST "Whether or not to enable test in the project" ON)

# 添加第三方依赖包
set(SQLITECPP_RUN_CPPLINT OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SQLiteCpp)

if(PKM_BUILD_TEST)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest)
    message(STATUS "cpkm: Test is enabled!")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp)

add_executable(cpkm ${SRC_FILES})
target_link_libraries(cpkm
  SQLiteCpp
  sqlite3
  pthread
)
