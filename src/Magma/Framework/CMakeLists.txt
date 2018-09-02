# ./src/Magma/Framework/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Magma_Framework_Source
    "*.hpp"
    "*.cpp"
	"*.h"
	"*.c"
)

# Generate config file
option (MAGMA_FRAMEWORK_USE_DIRECTX "Will the framework enable the use of DirectX for graphics?" OFF)
option (MAGMA_FRAMEWORK_USE_OPENGL "Will the framework enable the use of OpenGL for graphics?" ON)
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	set (MAGMA_FRAMEWORK_USE_DIRECTX 1)
	message(STATUS "Magma-Framework with DirectX enabled")
else()
	set (MAGMA_FRAMEWORK_USE_DIRECTX 0)
	message(STATUS "Magma-Framework with DirectX disabled")
endif()
if (MAGMA_FRAMEWORK_USE_OPENGL)
	set (MAGMA_FRAMEWORK_USE_OPENGL 1)
	message(STATUS "Magma-Framework with OpenGL enabled")
else()
	set (MAGMA_FRAMEWORK_USE_OPENGL 0)
	message(STATUS "Magma-Framework with OpenGL disabled")
endif()

option (MAGMA_FRAMEWORK_USE_OPENAL "Will the framework enable the use of OpenAL for audio?" ON)
if (MAGMA_FRAMEWORK_USE_OPENAL)
	set (MAGMA_FRAMEWORK_USE_OPENAL 1)
	message(STATUS "Magma-Framework with OpenAL enabled")
else()
	set (MAGMA_FRAMEWORK_USE_OPENAL 0)
	message(STATUS "Magma-Framework with OpenAL disabled")
endif()

option (MAGMA_FRAMEWORK_DEBUG "Will the framework perform debug checks?" ON)
if (MAGMA_FRAMEWORK_DEBUG)
	set (MAGMA_FRAMEWORK_DEBUG 1)
	message(STATUS "Magma-Framework Debug activated")
else()
	set (MAGMA_FRAMEWORK_DEBUG 0)
	message(STATUS "Magma-Framework Debug deactivated")
endif()

option (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM "Will the framework enable the use of Windows API for the file system functions?" ON)
if (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM)
	set (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM 1)
	message(STATUS "Magma-Framework with Windows FileSystem enabled")
else()
	set (MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM 0)
	message(STATUS "Magma-Framework with Windows FileSystem disabled")
endif()

option (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS "Will the framework use the Windows API for multi-threading support?" ON)
if (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS)
	set (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS 1)
	message(STATUS "Magma-Framework with windows threads enabled")
else()
	set (MAGMA_FRAMEWORK_USE_WINDOWS_THREADS 0)
	message(STATUS "Magma-Framework with windows threads disabled")
endif()

set (MAGMA_ROOT_DIRECTORY ${CMAKE_SOURCE_DIR})

configure_file(Config.h.in Config.h)

# Add files as library
add_library(Magma-Framework ${Magma_Framework_Source} ${CMAKE_CURRENT_BINARY_DIR}/Config.h)
set_target_properties (Magma-Framework PROPERTIES FOLDER Magma)

# Include config file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

# Include Magma libraries
include_directories(../../)

# Include external libraries
include_directories(../../../extern/freetype/include/)
include_directories(../../../extern/glm/)

# External libraries
target_link_libraries(Magma-Framework freetype)

if(MAGMA_FRAMEWORK_USE_OPENGL)
	include_directories(../../../extern/glew/include/)
	include_directories(../../../extern/glfw/include/)
	target_link_libraries(Magma-Framework glew_s)
	target_link_libraries(Magma-Framework glfw)
endif()

if(MAGMA_FRAMEWORK_USE_OPENAL)
	include_directories(../../../extern/openal-soft/include/)
	target_link_libraries(Magma-Framework OpenAL)
endif()
