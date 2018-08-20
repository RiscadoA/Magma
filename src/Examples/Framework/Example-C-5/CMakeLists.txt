# ./src/Examples/Framework/Example-C-5/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_C_5_Source
	"*.hpp"
    "*.cpp"
    "*.h"
    "*.c"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-C-5 WIN32 ${Example_C_5_Source})
else()
	add_executable(Example-C-5 ${Example_C_5_Source})
endif()

set_target_properties (Example-C-5 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)

# Link magma libraries
target_link_libraries(Example-C-5 Magma-Framework)
