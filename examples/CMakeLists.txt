add_executable(invert_colors src/invert_colors.cpp)
target_link_libraries(invert_colors libColorTool)

add_executable(remap_colors src/remap_colors.cpp)
target_link_libraries(remap_colors libColorTool)

add_custom_target(
	copy_examples ALL
	COMMAND
		${CMAKE_COMMAND} -E copy 
		${CMAKE_CURRENT_SOURCE_DIR}/source_palette.gpl
		${CMAKE_CURRENT_SOURCE_DIR}/source_image.tga
		${CMAKE_CURRENT_SOURCE_DIR}/src/invert_colors.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/remap_colors.cpp
		${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS
		source_palette.gpl
		source_image.tga
	BYPRODUCTS
		${CMAKE_CURRENT_BINARY_DIR}/source_palette.gpl
		${CMAKE_CURRENT_BINARY_DIR}/source_image.tga
		${CMAKE_CURRENT_BINARY_DIR}/invert_colors.cpp
		${CMAKE_CURRENT_BINARY_DIR}/remap_colors.cpp
	COMMENT
		"Copying files for running examples.")

add_custom_command(
	TARGET invert_colors
	POST_BUILD
	COMMAND
		invert_colors
	WORKING_DIRECTORY
		${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/source_image.tga
	COMMENT
		"Running invert_colors example")