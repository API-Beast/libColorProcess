add_executable(colortool_test color_tests.cpp image_tests.cpp math_tests.cpp palette_tests.cpp test_main.cpp yestest.cpp)
target_link_libraries(colortool_test libColorTool)
set_target_properties(colortool_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	OUTPUT_NAME run_tests)

add_custom_target(
	run_tests
	COMMAND colortool_test
	COMMENT
		"Running tests.")

message("-- Scaning for tests")
get_property(TEST_SOURCES TARGET colortool_test PROPERTY SOURCES)
set(REGEX_EXP "YES_TEST\\(([A-Za-z_0-9]+), *([A-Za-z_0-9]+) *\\)")
set(NUM_TESTS 0)
foreach(SOURCE IN LISTS TEST_SOURCES)
	file(READ "${SOURCE}" CONTENTS)
	string(REGEX MATCHALL "${REGEX_EXP}" FOUND_TESTS "${CONTENTS}")
	foreach(REGEX_HIT ${FOUND_TESTS})
		string(REGEX REPLACE ${REGEX_EXP} \\1 TEST_CATEGORY ${REGEX_HIT})
		string(REGEX REPLACE ${REGEX_EXP} \\2 TEST_NAME ${REGEX_HIT})
		add_test(NAME "${TEST_CATEGORY}::${TEST_NAME}"
					COMMAND colortool_test
					${TEST_CATEGORY}
					${TEST_NAME}
		)
		MATH(EXPR NUM_TESTS "${NUM_TESTS}+1")
	endforeach()
endforeach()
message("-- ${NUM_TESTS} test cases found")