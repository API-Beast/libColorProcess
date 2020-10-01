add_executable(colortool_test unit_tests.cpp yestest.cpp)
target_link_libraries(colortool_test ColorToolCore)
add_test(NAME colortool_test COMMAND colortool_test)