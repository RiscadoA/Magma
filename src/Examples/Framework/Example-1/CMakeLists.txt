# ./src/Examples/Example-1/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_1_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-1 WIN32 ${Example_1_Source})
else()
	add_executable(Example-1 ${Example_1_Source})
endif()

set_target_properties (Example-1 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)
include_directories(../../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-1 Magma-Framework)
