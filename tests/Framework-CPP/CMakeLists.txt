# ./src/Tests/Framework-CPP/CMakeLists.txt

file(GLOB_RECURSE files "*.cpp")

SET(CTEST_OUTPUT_ON_FAILURE TRUE)

foreach(file ${files})
	string(REGEX REPLACE "(^.*\\/|\\.[^.]*$)" "" file_without_ext ${file})
	set(file_without_ext Magma-Framework-CPP-Test_${file_without_ext})
	add_executable(${file_without_ext} ${file})
	target_link_libraries(${file_without_ext} Magma-Framework)
	add_test(NAME ${file_without_ext} COMMAND ${file_without_ext})
	set_tests_properties(${file_without_ext}
		PROPERTIES
		PASS_REGULAR_EXPRESSION "Test passed")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		FAIL_REGULAR_EXPRESSION "(Exception|Test failed)")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		TIMEOUT 120)
	set_target_properties(${file_without_ext} PROPERTIES FOLDER Tests)
	if (DEFINED OPENAL_DLLS)
		add_custom_command(TARGET ${file_without_ext} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${OPENAL_DLLS}
		$<TARGET_FILE_DIR:${file_without_ext}>)
	endif()
endforeach()