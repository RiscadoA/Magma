# ./src/Magma/Framework/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Magma_Framework_Source
    "*.hpp"
    "*.cpp"
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

option (MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG "Will the framework do debug checks on render devices?" ON)
if (MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG)
	set (MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG 1)
	message(STATUS "Magma-Framework Render Device Debug activated")
else()
	set (MAGMA_FRAMEWORK_RENDER_DEVICE_DEBUG 0)
	message(STATUS "Magma-Framework Render Device Debug deactivated")
endif()

option (MAGMA_FRAMEWORK_USE_STD_FILESYSTEM "Will the framework enable the use of standard library for the file system class?" ON)
if (MAGMA_FRAMEWORK_USE_STD_FILESYSTEM)
	set (MAGMA_FRAMEWORK_USE_STD_FILESYSTEM 1)
	message(STATUS "Magma-Framework with STD FileSystem enabled")
else()
	set (MAGMA_FRAMEWORK_USE_STD_FILESYSTEM 0)
	message(STATUS "Magma-Framework with STD FileSystem disabled")
endif()

configure_file(Config.hpp.in Config.hpp)

# Add files as library
add_library(Magma-Framework ${Magma_Framework_Source} ${CMAKE_CURRENT_BINARY_DIR}/Config.hpp)
set_target_properties (Magma-Framework PROPERTIES FOLDER Magma)

# Include config file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

# Include Magma libraries
include_directories(../../)

# Include external libraries
include_directories(../../../extern/freetype/include/)
include_directories(../../../extern/glew/include/)
include_directories(../../../extern/glfw/include/)
include_directories(../../../extern/glm/)
include_directories(../../../extern/openal-soft/include/)

# Link external libraries
target_link_libraries(Magma-Framework freetype)
target_link_libraries(Magma-Framework glew_s)
target_link_libraries(Magma-Framework glfw)
target_link_libraries(Magma-Framework OpenAL)
