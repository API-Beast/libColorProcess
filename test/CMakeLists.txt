add_executable(colortool_test unit_tests.cpp yestest.cpp)
target_link_libraries(colortool_test libColorTool)
set_target_properties(colortool_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
	OUTPUT_NAME run_tests)
add_test(NAME colortool_test COMMAND colortool_test)