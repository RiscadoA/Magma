# ./src/Examples/Example-6/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_6_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-6 WIN32 ${Example_6_Source})
else()
	add_executable(Example-6 ${Example_6_Source})
endif()

set_target_properties (Example-6 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../)
include_directories(../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-6 Magma-Framework)
