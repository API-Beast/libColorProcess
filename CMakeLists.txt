cmake_minimum_required(VERSION 3.16)
project(ColorToolCore LANGUAGES CXX)

# This is an internal CMake variable, we just expose it to the GUI.
# It affects all library that have no type (e.g. no SHARED or STATIC)
option(BUILD_SHARED_LIBS "Build ColorToolCore as shared library" OFF)

# We are using the project global variables instead of target properties so it applies to the tests too
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_library(ColorToolCore ColorToolCore.h Colors/Colors.h Colors/ColorSpaces.h Colors/ColorSpaces.cpp Colors/Colors.cpp)
target_include_directories(ColorToolCore PUBLIC .)

# Only do testing if not a sub-project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
	add_subdirectory(Tests)
endif()