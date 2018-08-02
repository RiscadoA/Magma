# ./src/Magma/Resources/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Magma_Resources_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-Resources ${Magma_Resources_Source})
set_target_properties (Magma-Resources PROPERTIES FOLDER Magma)

# Include config file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

# Include Magma libraries
include_directories(../../)

# Include external libraries
include_directories(../../../extern/glm/)

# Link libraries
target_link_libraries(Magma-Resources Magma-Framework)