# ./src/Examples/Example-4/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_4_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-4 WIN32 ${Example_4_Source})
else()
	add_executable(Example-4 ${Example_4_Source})
endif()

set_target_properties (Example-4 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../)
include_directories(../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-4 Magma-Framework)
