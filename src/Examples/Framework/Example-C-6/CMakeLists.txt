# ./src/Examples/Framework/Example-C-6/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_C_6_Source
	"*.hpp"
    "*.cpp"
    "*.h"
    "*.c"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-C-6 WIN32 ${Example_C_6_Source})
else()
	add_executable(Example-C-6 ${Example_C_6_Source})
endif()

set_target_properties (Example-C-6 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)

# Link magma libraries
target_link_libraries(Example-C-6 Magma-Framework)
