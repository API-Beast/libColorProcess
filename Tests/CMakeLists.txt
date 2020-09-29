add_executable(Tests Tests.cpp yestest.cpp)
target_link_libraries(Tests ColorToolCore)
add_test(NAME Colors.h COMMAND Tests)