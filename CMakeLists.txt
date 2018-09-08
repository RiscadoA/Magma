# ./CMakeLists.txt
cmake_minimum_required(VERSION 3.8)
project(Magma)
include(CTest)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add external dependencies
add_subdirectory(extern/)

# Add games and framework
add_subdirectory(src/)
