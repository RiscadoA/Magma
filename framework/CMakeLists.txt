# ./framework/CMakeLists.txt

# Should the framework support DirectX for graphics?
option (MAGMA_FRAMEWORK_USE_DIRECTX "Will the framework enable the use of DirectX for graphics?" OFF)
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	set (MAGMA_FRAMEWORK_USE_DIRECTX 1)
	message(STATUS "Magma-Framework with DirectX enabled")
else()
	set (MAGMA_FRAMEWORK_USE_DIRECTX 0)
	message(STATUS "Magma-Framework with DirectX disabled")
endif()

# Should the framework support OpenGL for graphics?
option (MAGMA_FRAMEWORK_USE_OPENGL "Will the framework enable the use of OpenGL for graphics?" ON)
if (MAGMA_FRAMEWORK_USE_OPENGL)
	set (MAGMA_FRAMEWORK_USE_OPENGL 1)
	message(STATUS "Magma-Framework with OpenGL enabled")
else()
	set (MAGMA_FRAMEWORK_USE_OPENGL 0)
	message(STATUS "Magma-Framework with OpenGL disabled")
endif()

# Should the framework support OpenAL for audio?
option (MAGMA_FRAMEWORK_USE_OPENAL "Will the framework enable the use of OpenAL for audio?" ON)
if (MAGMA_FRAMEWORK_USE_OPENAL)
	set (MAGMA_FRAMEWORK_USE_OPENAL 1)
	message(STATUS "Magma-Framework with OpenAL enabled")
else()
	set (MAGMA_FRAMEWORK_USE_OPENAL 0)
	message(STATUS "Magma-Framework with OpenAL disabled")
endif()

# Should the framework perform debug checks?
option (MAGMA_FRAMEWORK_DEBUG "Will the framework perform debug checks?" ON)
if (MAGMA_FRAMEWORK_DEBUG)
	set (MAGMA_FRAMEWORK_DEBUG 1)
	message(STATUS "Magma-Framework Debug activated")
else()
	set (MAGMA_FRAMEWORK_DEBUG 0)
	message(STATUS "Magma-Framework Debug deactivated")
endif()

# Should the framework use the Windows API for filesystem?
option (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM "Will the framework enable the use of Windows API for the file system functions?" ON)
if (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM)
	set (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM 1)
	message(STATUS "Magma-Framework with Windows FileSystem enabled")
else()
	set (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM 0)
	message(STATUS "Magma-Framework with Windows FileSystem disabled")
endif()

# Should the framework use the Windows API for threads?
option (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS "Will the framework use the Windows API for multi-threading support?" ON)
if (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS)
	set (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS 1)
	message(STATUS "Magma-Framework with windows threads enabled")
else()
	set (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS 0)
	message(STATUS "Magma-Framework with windows threads disabled")
endif()

# Generate config file
set (MAGMA_ROOT_DIRECTORY ${CMAKE_SOURCE_DIR})
configure_file(Config.h.in Config.h)

# Get source files
file(
	GLOB_RECURSE Magma_Framework_Source
    "*.hpp"
    "*.cpp"
	"*.h"
	"*.c"
)

# Add files as library
add_library(Magma-Framework ${Magma_Framework_Source} ${CMAKE_CURRENT_BINARY_DIR}/Config.h)
target_compile_features(Magma-Framework PUBLIC cxx_std_17)
set_target_properties (Magma-Framework PROPERTIES FOLDER Magma)

# Include config file
target_include_directories(Magma-Framework PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/)

# Include public headers
target_include_directories(Magma-Framework PUBLIC include)

# Add freetype
find_package(freetype REQUIRED)
target_link_libraries(Magma-Framework PRIVATE Freetype::Freetype)

# Add OpenAL if needed
if(MAGMA_FRAMEWORK_USE_OPENAL)
	find_package(OpenAL REQUIRED)
	target_include_directories(Magma-Framework PRIVATE ${OPENAL_INCLUDE_DIR})
	target_link_libraries(Magma-Framework PRIVATE ${OPENAL_LIBRARY})
	
	if (DEFINED OPENAL_DLLS)
		add_custom_command(TARGET Magma-Framework POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${OPENAL_DLLS}
		$<TARGET_FILE_DIR:Magma-Framework>)
	endif()
endif()

# Add GLEW and GLFW if needed
if(MAGMA_FRAMEWORK_USE_OPENGL)
	find_package(OpenGL REQUIRED)
	target_link_libraries(Magma-Framework PUBLIC OpenGL::GL)

	set(GLEW_USE_STATIC_LIBS ON)
	find_package(GLEW REQUIRED)
	target_link_libraries(Magma-Framework PUBLIC GLEW::GLEW)

	find_package(GLFW REQUIRED)
	target_include_directories(Magma-Framework PRIVATE ${GLFW_INCLUDE_DIR})
	target_link_libraries(Magma-Framework PUBLIC ${GLFW_LIBRARIES})
endif()

# Add ZLIB
find_package(ZLIB REQUIRED)
target_link_libraries(Magma-Framework PRIVATE ZLIB::ZLIB)

# Add GLM and stb_image.h
target_include_directories(
	Magma-Framework
	PRIVATE
		${CMAKE_SOURCE_DIR}/extern/
	PUBLIC
		${CMAKE_SOURCE_DIR}/extern/glm/
)
