# ./engine/CMakeLists.txt

# Get source files
file(
	GLOB_RECURSE Magma_Engine_Source
    "src/*.hpp"
    "src/*.cpp"
	"src/*.h"
	"src/*.c"
	"include/*.hpp"
	"include/*.h"
)

# Add files as library
add_library(Magma-Engine ${Magma_Engine_Source})
target_compile_features(Magma-Engine PUBLIC cxx_std_17)
target_link_libraries(Magma-Engine PUBLIC Magma-Framework)

# Add file filters
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)
assign_source_group(${Magma_Engine_Source})

# Include public headers
target_include_directories(Magma-Engine PUBLIC include)