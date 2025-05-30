set(SRC_FILES "")

file(GLOB_RECURSE SRC_FILES Src/*.cpp)

add_library(cpkm_lib ${SRC_FILES} )
target_include_directories(cpkm_lib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)
target_link_libraries(cpkm_lib
        SQLiteCpp
        sqlite3
        pthread
)