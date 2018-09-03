# ./src/Tests/Framework-C/CMakeLists.txt

file(GLOB_RECURSE files "*.c")

SET(CTEST_OUTPUT_ON_FAILURE TRUE)

foreach(file ${files})
	string(REGEX REPLACE "(^.*\\/|\\.[^.]*$)" "" file_without_ext ${file})
	add_executable(${file_without_ext} ${file})
	target_link_libraries(${file_without_ext} Magma-Framework)
	include_directories(${CMAKE_CURRENT_LIST_DIR}/../../)
	include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../extern/glm/)
	add_test(${file_without_ext} ${file_without_ext})
	set_tests_properties(${file_without_ext}
		PROPERTIES
		PASS_REGULAR_EXPRESSION "Test passed")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		FAIL_REGULAR_EXPRESSION "(Exception|Test failed)")
	set_tests_properties(${file_without_ext}
		PROPERTIES
		TIMEOUT 120)
endforeach()