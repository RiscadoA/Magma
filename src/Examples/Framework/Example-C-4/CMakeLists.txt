# ./src/Examples/Framework/Example-C-4/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_C_4_Source
	"*.hpp"
    "*.cpp"
    "*.h"
    "*.c"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-C-4 WIN32 ${Example_C_4_Source})
else()
	add_executable(Example-C-4 ${Example_C_4_Source})
endif()

set_target_properties (Example-C-4 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)

# Link magma libraries
target_link_libraries(Example-C-4 Magma-Framework)
