# ./extern/CMakeLists.txt

add_subdirectory(freetype/)
add_subdirectory(glew/build/cmake/)
add_subdirectory(glfw/)
add_subdirectory(glm/)
unset(LIB_SUFFIX CACHE)
add_subdirectory(openal-soft/)
add_subdirectory(zlib/ ${CMAKE_CURRENT_BINARY_DIR}/zlib)
