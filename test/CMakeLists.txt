add_executable(colortool_test color_tests.cpp image_tests.cpp math_tests.cpp palette_tests.cpp test_main.cpp yestest.cpp)
target_link_libraries(colortool_test libColorTool)
set_target_properties(colortool_test PROPERTIES OUTPUT_NAME run_tests)
add_test(NAME colortool_test COMMAND colortool_test)