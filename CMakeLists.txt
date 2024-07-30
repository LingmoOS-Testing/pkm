cmake_minimum_required(VERSION 3.5.0)
project(pkm VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp)

add_executable(pkm ${SRC_FILES})
