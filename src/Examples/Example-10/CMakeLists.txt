# ./src/Examples/Example-10/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_10_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-10 WIN32 ${Example_10_Source})
else()
	add_executable(Example-10 ${Example_10_Source})
endif()

set_target_properties (Example-10 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../)
include_directories(../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-10 Magma-Framework)
target_link_libraries(Example-10 Magma-GUI)
