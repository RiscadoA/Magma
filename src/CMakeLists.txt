# ./src/CMakeLists.txt
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

add_subdirectory(Magma/)
add_subdirectory(Examples/)
add_subdirectory(Tests/)
