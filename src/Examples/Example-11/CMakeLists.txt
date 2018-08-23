# ./src/Examples/Example-11/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_11_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
add_executable(Example-11 ${Example_11_Source})

set_target_properties (Example-11 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../)
include_directories(../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-11 Magma-Framework)
target_link_libraries(Example-11 Magma-Resources)
target_link_libraries(Example-11 Magma-GUI)
