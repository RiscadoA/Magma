# ./src/Examples/Example-8/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_8_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-8 WIN32 ${Example_8_Source})
else()
	add_executable(Example-8 ${Example_8_Source})
endif()

set_target_properties (Example-8 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)
include_directories(../../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-8 Magma-Framework)
