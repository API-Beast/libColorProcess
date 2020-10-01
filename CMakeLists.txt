cmake_minimum_required(VERSION 3.16)
project(ColorToolCore LANGUAGES CXX)

# This is an internal CMake variable, we just expose it to the GUI.
# It affects all library that have no type (e.g. no SHARED or STATIC)
option(BUILD_SHARED_LIBS "Build ColorToolCore as shared library" OFF)
#option(CMAKE_LINK_SEARCH_START_STATIC "Prefer linking to static libraries" OFF)

# We are using the project global variables instead of target properties so it applies to the tests too
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(HEADER_FILES ColorToolCore.h)
list(APPEND HEADER_FILES Colors/Analysis.h Colors/ColorPalette.h Colors/Colors.h Colors/ColorSpaces.h)
list(APPEND HEADER_FILES Math/Vector3.h Math/Vector3Mixin.h)

set(SOURCE_FILES Colors/Colors.cpp Colors/ColorSpaces.cpp)

add_library(ColorToolCore ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(ColorToolCore PUBLIC .)
set_target_properties(ColorToolCore PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

install(TARGETS ColorToolCore)

# Only do testing if not a sub-project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
	add_subdirectory(test)
endif()