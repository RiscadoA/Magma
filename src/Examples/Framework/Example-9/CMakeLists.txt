# ./src/Examples/Example-9/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_9_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-9 WIN32 ${Example_9_Source})
else()
	add_executable(Example-9 ${Example_9_Source})
endif()

set_target_properties (Example-9 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)
include_directories(../../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-9 Magma-Framework)
