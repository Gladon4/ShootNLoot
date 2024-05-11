cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME ShootNLoot)
set(RAYLIB_VERSION 5.0)

project(${PROJECT_NAME})

find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c11")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB_RECURSE source_files "${source_dir}/*.c")

add_executable(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include/lib/")
target_link_libraries(${PROJECT_NAME} raylib)
