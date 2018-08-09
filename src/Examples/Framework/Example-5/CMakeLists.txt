# ./src/Examples/Example-5/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_5_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-5 WIN32 ${Example_5_Source})
else()
	add_executable(Example-5 ${Example_5_Source})
endif()

set_target_properties (Example-5 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)
include_directories(../../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-5 Magma-Framework)
