# ./CMakeLists.txt
cmake_minimum_required(VERSION 3.8)
project(Magma)
include(CTest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build Framework
add_subdirectory(framework/)

# Build examples
add_subdirectory(examples/)
