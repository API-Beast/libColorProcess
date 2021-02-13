cmake_minimum_required(VERSION 3.16)
project(libColorProcess LANGUAGES CXX C)

# This is an internal CMake variable, we just expose it to the GUI.
# It affects all library that have no type (e.g. no SHARED or STATIC)
option(BUILD_SHARED_LIBS "Build libColorProcess as shared library" OFF)
#option(CMAKE_LINK_SEARCH_START_STATIC "Prefer linking to static libraries" OFF)

# We are using the project global variables instead of target properties so it applies to the tests too
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(SOURCE_FILES Colors/Colors.cpp Colors/ColorSpaces.cpp Colors/GenericColor.cpp)
list(APPEND SOURCE_FILES Image/ImageImport.cpp)
list(APPEND SOURCE_FILES Palette/PaletteImport.cpp)

set(HEADER_FILES libColorProcess.h)
list(APPEND HEADER_FILES Palette/Palette.h Palette/PaletteImport.h Palette/Palette_Sort.hpp Palette/Palette_MedianSplit.hpp)
list(APPEND HEADER_FILES Colors/Stats.h Colors/Colors.h Colors/ColorSpaces.h)
list(APPEND HEADER_FILES Math/Vector3.h)

add_library(libColorProcess ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(libColorProcess PUBLIC . Image/dependencies/)
set_target_properties(libColorProcess PROPERTIES
	WINDOWS_EXPORT_ALL_SYMBOLS ON
	PREFIX ""
	)

# Enable vectorization
if(CMAKE_COMPILER_IS_GNUCXX)
	option(LIBCOLORTOOL_AUTO_VECTORIZATION "Try to auto-vectorize all targets linking libColorProcess." ON)
	if(LIBCOLORTOOL_AUTO_VECTORIZATION)
		target_compile_options(libColorProcess PUBLIC -ftree-loop-vectorize -fopt-info-vec-all)
	endif()
endif()

install(TARGETS libColorProcess)

# Only build examples and testing if not a sub-project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
	add_subdirectory(test)
	
	option(BUILD_EXAMPLES "Build libColorProcess examples" ON)
	if(BUILD_EXAMPLES)
		add_subdirectory(examples)
	endif()
endif()