# ./src/Examples/Example-7/CMakeLists.txt

# Get all files
file(GLOB_RECURSE Example_7_Source
    "*.hpp"
    "*.cpp"
)

# Add files as executable
if (MAGMA_FRAMEWORK_USE_DIRECTX)
	add_executable(Example-7 WIN32 ${Example_7_Source})
else()
	add_executable(Example-7 ${Example_7_Source})
endif()

set_target_properties (Example-7 PROPERTIES FOLDER Examples)

# Include magma libraries
include_directories(../../../)
include_directories(../../../../extern/glm/)

# Link magma libraries
target_link_libraries(Example-7 Magma-Framework)
