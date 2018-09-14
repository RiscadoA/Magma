# ./src/Magma/Engine/Core/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Magma_Engine_Core_Source
    "*.hpp"
    "*.cpp"
	"*.h"
	"*.c"
)

# Add files as executable
add_executable(Magma-Engine-Core ${Magma_Engine_Core_Source})
set_target_properties (Magma-Engine-Core PROPERTIES FOLDER Magma)

# Include and link Magma Framework
include_directories(../../../)
target_link_libraries(Magma-Engine-Core Magma-Framework)
