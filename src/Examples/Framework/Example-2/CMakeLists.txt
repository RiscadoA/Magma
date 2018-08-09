# ./src/Examples/Example-2/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_2_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-2 WIN32 ${Example_2_Source})
else()
	add_executable(Example-2 ${Example_2_Source})
endif()

set_target_properties (Example-2 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)
include_directories(../../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-2 Magma-Framework)
