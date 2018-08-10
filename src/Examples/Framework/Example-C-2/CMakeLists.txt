# ./src/Examples/Framework/Example-C-2/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_C_2_Source
	"*.hpp"
    "*.cpp"
    "*.h"
    "*.c"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-C-2 WIN32 ${Example_C_2_Source})
else()
	add_executable(Example-C-2 ${Example_C_2_Source})
endif()

set_target_properties (Example-C-2 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)

# Link magma libraries
target_link_libraries(Example-C-2 Magma-Framework)
