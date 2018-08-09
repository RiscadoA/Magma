# ./src/Examples/Framework/Example-C-1/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_C_1_Source
    "*.h"
    "*.c"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-C-1 WIN32 ${Example_C_1_Source})
else()
	add_executable(Example-C-1 ${Example_C_1_Source})
endif()

set_target_properties (Example-C-1 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)

# Link magma libraries
target_link_libraries(Example-C-1 Magma-Framework)
