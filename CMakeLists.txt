cmake_minimum_required(VERSION 3.15.0)
project(cpkm VERSION 0.1.0 LANGUAGES C CXX)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(PKM_BUILD_TEST "Whether or not to enable test in the project" ON)
option(PKM_ENABLE_DEBUG "Enable debug ouput." ON)

if(PKM_ENABLE_DEBUG)
    add_compile_definitions("-DCPKM_ENABLE_DEBUG")
endif()

# 添加第三方依赖包
set(SQLITECPP_RUN_CPPLINT OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SQLiteCpp)

# If you only include this third party in PRIVATE source files, you do not need to install it
# when your main project gets installed.
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json)

if(PKM_BUILD_TEST)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/googletest)
endif()

add_subdirectory(libcpm)

add_subdirectory(cpm)

if(PKM_BUILD_TEST)
    message(STATUS "cpkm: Test is enabled!")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
