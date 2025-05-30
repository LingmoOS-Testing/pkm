set(SRC_FILES "")
include_directories(Inc)

file(GLOB_RECURSE SRC_FILES Src/*.cpp)

add_executable(cpkm ${SRC_FILES} )
target_link_libraries(cpkm
        cpkm_lib
)