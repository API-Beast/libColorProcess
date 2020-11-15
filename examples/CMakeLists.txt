add_executable(invert_colors src/invert_colors.cpp)
target_link_libraries(invert_colors libColorTool)

add_executable(basic_remap src/basic_remap.cpp)
target_link_libraries(basic_remap libColorTool)

add_executable(smart_remap src/smart_remap.cpp)
target_link_libraries(smart_remap libColorTool)

add_custom_target(
	copy_examples ALL
	COMMAND
		${CMAKE_COMMAND} -E copy 
		${CMAKE_CURRENT_SOURCE_DIR}/source_palette.gpl
		${CMAKE_CURRENT_SOURCE_DIR}/source_image.tga
		${CMAKE_CURRENT_SOURCE_DIR}/src/invert_colors.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/basic_remap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/smart_remap.cpp
		${CMAKE_CURRENT_BINARY_DIR}
	COMMENT
		"Copying files for running examples.")

add_custom_target(
	run_examples
	COMMAND invert_colors
	COMMAND basic_remap
	COMMAND smart_remap
	DEPENDS
		copy_examples
		invert_colors
		basic_remap
		smart_remap
	BYPRODUCTS
		${CMAKE_CURRENT_BINARY_DIR}/invert_colors.tga
		${CMAKE_CURRENT_BINARY_DIR}/basic_remap.tga
		${CMAKE_CURRENT_BINARY_DIR}/smart_remap.tga
	COMMENT
		"Running examples to generate images.")