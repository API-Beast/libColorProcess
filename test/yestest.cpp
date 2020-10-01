
#include "yestest.h"

using namespace YesTest;

bool YesTest::register_test(Test t)
{
	std::vector<Test>& v = get_test_list();
	v.push_back(t);
	return true;
};

std::vector<Test>& YesTest::get_test_list()
{
	static std::vector<Test> vector;
	return vector;
}

bool YesTest::Test::run()
{
	int errors = 0;
	try
	{
		if(func)
			func(&errors);
	}
	catch(std::exception& e)
	{
		errors += 1;
		printf("  Exception: %s", e.what());
	}
	catch(...)
	{
		errors += 1;
		printf("  Exception: Unknown exception.");
	}
	return errors == 0;
}

int YesTest::run_all_tests()
{
	std::vector<Test> failed_tests;
	std::vector<Test> all_tests = YesTest::get_test_list();

	printf("%s[==========]%s Running %u test cases.\n", CLR_GREEN, CLR_RESET, all_tests.size());
	for(Test current_test : all_tests)
	{
		printf("%s[ RUN      ]%s %s\n", CLR_GREEN, CLR_RESET, current_test.name);
		bool result = current_test.run();
		if(result == true)
			printf("%s[       OK ]%s %s\n", CLR_GREEN, CLR_RESET, current_test.name);
		else
		{
			printf("%s[  FAILED  ]%s %s\n", CLR_RED, CLR_RESET, current_test.name);
			failed_tests.push_back(current_test);
		}
	}
	printf("%s[==========]%s %u test cases ran.\n", CLR_GREEN, CLR_RESET, all_tests.size());
	if(failed_tests.size() > 0)
	{
		printf("%s[  FAILED  ]%s %u tests, listed below:\n", CLR_RED, CLR_RESET, failed_tests.size());
		for(Test failed_test : failed_tests)
			printf("%s[  FAILED  ]%s %s\n", CLR_RED, CLR_RESET, failed_test.name);
	}

	return failed_tests.size();
}