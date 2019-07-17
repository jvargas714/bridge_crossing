cmake_minimum_required(VERSION 3.10)
project(scala_computing)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3")
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(testing)