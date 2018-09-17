# ./examples/Framework-CPP/CMakeLists.txt

file(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(file ${files})
	if (${file} MATCHES ".+\.cpp")
		string(REPLACE "/" "_" executableName ${file})
		set (executableName "Magma-Framework-CPP_${executableName}")
		message(STATUS ${executableName} " " ${file})
		add_executable(${executableName} ${file})
		target_link_libraries(${executableName} Magma-Framework)
		set_target_properties(${executableName} PROPERTIES FOLDER Examples/Framework-CPP)
		if (DEFINED OPENAL_DLLS)
			add_custom_command(TARGET ${executableName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${OPENAL_DLLS}
			$<TARGET_FILE_DIR:${executableName}>)
		endif()
	endif()
endforeach()
