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
	message("Magma-Framework with DirectX enabled")
else()
	set (MAGMA_FRAMEWORK_USE_DIRECTX 0)
	message("Magma-Framework with DirectX disabled")
endif()
if (MAGMA_FRAMEWORK_USE_OPENGL)
	set (MAGMA_FRAMEWORK_USE_OPENGL 1)
	message("Magma-Framework with OpenGL enabled")
else()
	set (MAGMA_FRAMEWORK_USE_OPENGL 0)
	message("Magma-Framework with OpenGL disabled")
endif()

option (MAGMA_FRAMEWORK_CONTEXT_DEBUG "Will the framework do debug checks on graphic contexts?" ON)
if (MAGMA_FRAMEWORK_CONTEXT_DEBUG)
	set (MAGMA_FRAMEWORK_CONTEXT_DEBUG 1)
	message("Magma-Framework Context Debug activated")
else()
	set (MAGMA_FRAMEWORK_CONTEXT_DEBUG 0)
	message("Magma-Framework Context Debug deactivated")
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

# Link external libraries
target_link_libraries(Magma-Framework freetype)
target_link_libraries(Magma-Framework glew_s)
target_link_libraries(Magma-Framework glfw)