# ./src/Magma/GUI/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Magma_GUI_Source
    "*.hpp"
    "*.cpp"
)

# Add files as library
add_library(Magma-GUI ${Magma_GUI_Source})
set_target_properties (Magma-GUI PROPERTIES FOLDER Magma)

# Include config file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

# Include Magma libraries
include_directories(../../)

# Include external libraries
include_directories(../../../extern/glm/)

# Link libraries
target_link_libraries(Magma-GUI Magma-Framework)