# ./src/Tetris/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Tetris_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Tetris WIN32 ${Tetris_Source})
else()
	add_executable(Tetris ${Tetris_Source})
endif()

set_target_properties (Tetris PROPERTIES FOLDER Games)

# Include magma libraries
include_directories(../)
include_directories(../../../extern/glm/)

# Link magma libraries
target_link_libraries(Tetris Magma-Framework)
