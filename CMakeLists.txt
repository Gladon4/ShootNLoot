cmake_minimum_required(VERSION 3.2)

project(LootNShoot)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c11")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB_RECURSE source_files "${source_dir}/*.c")

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES IMPORTED_LOCATION "/home/alex/raylib/src/libraylib.so")

add_executable(LootNShoot ${source_files})

target_include_directories(LootNShoot PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(LootNShoot PUBLIC "${PROJECT_SOURCE_DIR}/include/lib/")
target_link_libraries(LootNShoot raylib)
