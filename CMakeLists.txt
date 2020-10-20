cmake_minimum_required(VERSION 3.16)
project(libColorTool LANGUAGES CXX C)

# This is an internal CMake variable, we just expose it to the GUI.
# It affects all library that have no type (e.g. no SHARED or STATIC)
option(BUILD_SHARED_LIBS "Build libColorTool as shared library" OFF)
#option(CMAKE_LINK_SEARCH_START_STATIC "Prefer linking to static libraries" OFF)

# We are using the project global variables instead of target properties so it applies to the tests too
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(SOURCE_FILES Colors/Colors.cpp Colors/ColorSpaces.cpp Colors/GenericColor.cpp)
list(APPEND SOURCE_FILES Image/ImageImport.cpp Image/dependencies/spng.c Image/dependencies/miniz.c)

set(HEADER_FILES libColorTool.h)
list(APPEND HEADER_FILES Palette/Palette.h Palette/Palette_Sort.hpp Palette/Palette_MedianSplit.hpp)
list(APPEND HEADER_FILES Colors/Stats.h Colors/Colors.h Colors/ColorSpaces.h)
list(APPEND HEADER_FILES Math/Vector3.h)

add_library(libColorTool ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(libColorTool PUBLIC . Image/dependencies/)
set_target_properties(libColorTool PROPERTIES
	WINDOWS_EXPORT_ALL_SYMBOLS ON
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	PREFIX ""
	)

# Enable vectorization
if(CMAKE_COMPILER_IS_GNUCXX)
	option(LIBCOLORTOOL_VECTORIZE "Try to vectorize all targets linking libColorTool." ON)
	if(LIBCOLORTOOL_VECTORIZE)
		# The "-march=native" is experimental for now, in the future we will use target_clones(...) gcc option to generate SIMD instructions for the targets that make sense.
		target_compile_options(libColorTool PUBLIC -march=native -ftree-vectorize -ftree-loop-vectorize -fopt-info-vec)
	endif()
endif()

install(TARGETS libColorTool)

# Only build examples and testing if not a sub-project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
	add_subdirectory(test)
	
	option(BUILD_EXAMPLES "Build libColorTool examples" ON)
	if(BUILD_EXAMPLES)
		add_subdirectory(examples)
	endif()
endif()