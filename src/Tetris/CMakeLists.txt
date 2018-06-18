# ./src/Tetris/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Tetris_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
add_executable(Tetris ${Tetris_Source})
set_target_properties (Tetris PROPERTIES FOLDER Games)

# Include magma libraries
include_directories(../)

# Link magma libraries
target_link_libraries(Tetris Magma-Framework)
