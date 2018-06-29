# ./CMakeLists.txt

cmake_minimum_required(VERSION 3.8)

# Add external dependencies
add_subdirectory(extern/)

# Add games and framework
project (Simple-Game)

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

add_subdirectory(src/)