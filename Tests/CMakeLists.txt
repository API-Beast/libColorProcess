add_executable(ColorsTest Colors_Test.cpp)
target_link_libraries(ColorsTest ColorToolCore)
add_test(NAME Colors.h COMMAND ColorsTest)