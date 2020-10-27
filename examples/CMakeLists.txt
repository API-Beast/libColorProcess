add_executable(palette_example palette_example.cpp HtmlExporter.cpp HtmlExporter.h)
target_link_libraries(palette_example libColorTool)
set_target_properties(palette_example PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(invert_colors invert_colors.cpp)
target_link_libraries(invert_colors libColorTool)
set_target_properties(invert_colors PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_command(
	TARGET palette_example
	POST_BUILD
	COMMAND
		${CMAKE_COMMAND} -E copy 
		${CMAKE_CURRENT_SOURCE_DIR}/style.css 
		${CMAKE_CURRENT_SOURCE_DIR}/palette.gpl
		${CMAKE_CURRENT_SOURCE_DIR}/test_image.png
		${CMAKE_CURRENT_SOURCE_DIR}/test_image.tga
		${CMAKE_BINARY_DIR}/bin
	DEPENDS
		style.css
		palette.gpl
		test_image.png
		test_image.tga
	BYPRODUCTS
		${CMAKE_BINARY_DIR}/bin/style.css
		${CMAKE_BINARY_DIR}/bin/palette.gpl
		${CMAKE_BINARY_DIR}/bin/test_image.png
		${CMAKE_BINARY_DIR}/test_image.tga
	COMMENT
		"Copying files for running examples.")