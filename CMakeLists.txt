cmake_minimum_required(VERSION 3.15.0)
project(installer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find 3rd party libs
# include(FindLibArchive)
# if (NOT LibArchive_FOUND)
#     message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires libarchive to compile!")
# else ()
#     include_directories(${LIBXML2_INCLUDE_DIRS})
#     include_directories(${LIBXML2_INCLUDE_DIR})
#     link_libraries(${LIBXML2_LIBRARIES})
#     add_compile_definitions(${LIBXML2_DEFINITIONS})
# endif ()

include(FindLibXar)
if (NOT LibXar_FOUND)
    message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires libxar to compile!")
else ()
    include_directories(${LibXar_INCLUDE_DIRS})
    link_libraries(${LibXar_LIBRARIES})
endif ()

include(FindLibXml2)
if (NOT LibXml2_FOUND)
message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires libxml2 to compile!")
else ()
    include_directories(${LibArchive_INCLUDE_DIRS})
    link_libraries(${LibArchive_LIBRARIES})
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Inc)

file(GLOB_RECURSE SRC_FILES "Src/*.cpp" "Src/**/*.cpp")

add_executable(installer ${SRC_FILES})
