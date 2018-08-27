# ./src/Examples/Framework/CMakeLists.txt

file(GLOB_RECURSE sub-dirs LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(dir ${sub-dirs})
	message(STATUS ${dir})
	if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${dir})
		if (${dir} MATCHES "Example-.+")
			file(GLOB_RECURSE files 
				"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.hpp"
				"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.cpp"
				"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.h"
				"${CMAKE_CURRENT_LIST_DIR}/${dir}/*.c")
			message(STATUS ${files})

			string(REPLACE "/" "_" dir ${dir})
			add_executable(${dir} ${files})
			target_link_libraries(${dir} Magma-Framework)
			include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
			include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../extern/glm/)
		endif()
	endif()
endforeach()