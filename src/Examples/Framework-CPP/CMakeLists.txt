# ./src/Examples/Framework-CPP/CMakeLists.txt

file(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(file ${files})
	if (${file} MATCHES ".+\.cpp")
		string(REPLACE "/" "_" executableName ${file})
		set (executableName "Magma-Framework-CPP_${executableName}")
		message(STATUS ${executableName} " " ${file})
		add_executable(${executableName} ${file})
		target_link_libraries(${executableName} Magma-Framework)
		include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
		include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../extern/glm/)
	endif()
endforeach()
