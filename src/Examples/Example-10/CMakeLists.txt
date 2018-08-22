# ./src/Examples/Example-10/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_10_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
add_executable(Example-10 ${Example_10_Source})

set_target_properties (Example-10 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../)
include_directories(../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-10 Magma-Framework)
target_link_libraries(Example-10 Magma-Resources)
