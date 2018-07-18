# ./CMakeLists.txt

cmake_minimum_required(VERSION 3.8)

# Add external dependencies
add_subdirectory(extern/)

# Add games and framework
add_subdirectory(src/)