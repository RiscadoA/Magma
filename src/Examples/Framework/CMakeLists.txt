# ./src/Examples/Framework/CMakeLists.txt

file(GLOB sub-dirs RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(dir ${sub-dirs})
	if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${dir})
		file(GLOB_RECURSE files 
			"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.h"
			"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.c")
		message(STATUS ${files})
		add_executable(${dir} ${files})
		target_link_libraries(${dir} Magma-Framework)
		include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
		include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../extern/glm/)
	endif()
endforeach()