#pragma once
#include <cstddef>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cfloat>

#define YES_TEST(CATEGORY,NAME)\
static void yes_test_run_##CATEGORY##_##NAME(int* yes_test_errors);\
static bool yes_test_##CATEGORY##_##NAME##_registered = YesTest::register_test({#CATEGORY, #CATEGORY "::" #NAME, &yes_test_run_##CATEGORY##_##NAME});\
static void yes_test_run_##CATEGORY##_##NAME(int* yes_test_errors)

#define CLR_RESET "\033[0m"
#define CLR_RED "\033[31m"
#define CLR_GREEN "\033[32m"
#define CLR_CYAN "\033[96m"
#define CLR_YELLOW "\033[93m"

#define YES_MAIN()\
int main(int argc, char** argv)\
{\
	return YesTest::run_all_tests();\
}

namespace YesTest
{
	using TestFunctionPointer = void (*)(int*);
	struct Test
	{
		const char* category;
		const char* name;
		TestFunctionPointer func;
		bool run();
	};

	int run_all_tests();
	std::vector<Test>& get_test_list();
	bool register_test(Test t);

	inline void print_value(bool b)         { if(b) printf(" true"); else printf("false");};
	inline void print_value(int v)          { printf("%5d", v); };
	inline void print_value(unsigned int v) { printf("%5u", v); };
	inline void print_value(float v)        { printf("%5.3f", double(v)); };
	inline void print_value(double v)       { printf("%5.3f", v); };
	inline void print_value(long double v)  { printf("%5.3Lf", v); };
	inline void print_value(long long int v){ printf("%5.3lld", v); };
	inline void print_value(long long unsigned int v){ printf("%5llu", v); };
	template<typename T>
	inline void print_value(T* ptr){ printf("%5p", ptr); };
	inline void print_value(std::nullptr_t){ printf("nullptr"); };

	template<typename A, typename B>
	inline void print_value(A arg1, B arg2)
	{
		print_value(static_cast<typename std::decay<A>::type>(arg1));
		printf(", ");
		print_value(static_cast<typename std::decay<B>::type>(arg2));
	}

	template<typename A, typename B, typename... Args>
	inline void print_value(A arg1, B arg2, Args... args)
	{
		print_value(static_cast<typename std::decay<A>::type>(arg1));
		printf(", ");
		print_value(static_cast<typename std::decay<B>::type>(arg2));
		printf(", ");
		print_value(args...);
	}

	// ULP = unit of least precision, how many discrete steps floats differ
	inline int ulp_difference(float a, float b)
	{
		int ai = *(int*)&a;
		if(ai < 0) ai = 0x80000000 - ai;
		int bi = *(int*)&b;
		if(bi < 0) bi = 0x80000000 - bi;
		float diff = std::abs(a - b);
		// We calculate both bitwise ULP difference as well as EPSILON difference.
		// This is because the bitwise difference can be enormous for numbers very close to 0.
		return std::min(std::abs(ai - bi), int(diff / FLT_EPSILON) / 5);
	}
};

#define YES_PRINT_FAILURE(MSG) printf("%s%s:%u%s: %serror:%s " MSG "\n", CLR_CYAN, __FILE__, __LINE__, CLR_RESET, CLR_RED, CLR_RESET)

#define EXPECT_SIMILAR(x, y, ulps)\
{\
	auto val_a = (x); auto val_b = (y);\
	int difference = YesTest::ulp_difference(val_a, val_b);\
	if(difference > ulps)\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("expectation not fulfilled");\
		printf(" -> |   Expected: " #x " ~= " #y "\n");\
		printf(" -> |     Actual: %s", CLR_YELLOW);\
		YesTest::print_value(val_a);\
		printf(" !~= ");\
		YesTest::print_value(val_b);\
		printf("%s\n", CLR_RESET);\
		printf(" -> |  Precision: %d ULPs (Threshold: " #ulps " ULPs)\n", difference);\
	}\
}

#define EXPECT_UNSIMILAR(x, y, ulps)\
{\
	auto val_a = (x); auto val_b = (y);\
	int difference = ulp_difference(val_a, val_b);\
	if(difference < ulps)\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("expectation not fulfilled");\
		printf(" -> |   Expected: "#x" !~= " #y"\n");\
		printf(" -> |     Actual: %s", CLR_YELLOW);\
		YesTest::print_value(val_a);\
		printf(" ~= ");\
		YesTest::print_value(val_b);\
		printf("%s\n", CLR_RESET);\
		printf(" -> |  Precision: %d ULPs (Threshold: " #ulps " ULPs)\n", difference);\
	}\
}

#define EXPECT_OP(x, y, OPER, FAILURE)\
{\
	auto val_a = (x); auto val_b = (y);\
	if(!(val_a OPER val_b))\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("expectation not fulfilled");\
		printf(" -> |   Expected: "#x" "#OPER" "#y"\n");\
		printf(" -> |     Actual: %s", CLR_YELLOW);\
		YesTest::print_value(val_a);\
		printf(" " #FAILURE " ");\
		YesTest::print_value(val_b);\
		printf("%s\n", CLR_RESET);\
	}\
}

#define EXPECT_TRUE(x)\
{\
	auto val_a = (x);\
	if(!(val_a))\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("expectation not fulfilled");\
		printf(" -> |   Expected: "#x"\n");\
		printf(" -> |     Actual: %s", CLR_YELLOW);\
		YesTest::print_value(val_a);\
		printf("%s\n", CLR_RESET);\
	}\
}

#define EXPECT_FALSE(x)\
{\
	auto val_a = (x);\
	if((val_a))\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("expectation not fulfilled");\
		printf(" -> |   Expected: "#x" is false\n");\
		printf(" -> |     Actual: %s", CLR_YELLOW);\
		YesTest::print_value(val_a);\
		printf("%s is true\n", CLR_RESET);\
	}\
}

#define PRINT_INFO(x)\
{\
	if(*yes_test_errors)\
	{\
		auto val = (x);\
		printf(" -> |       Info: %8s = %s", #x, CLR_CYAN);\
		YesTest::print_value(val);\
		printf("%s\n", CLR_RESET);\
	}\
}

#define EXPECT_EQ(x, y) EXPECT_OP(x, y, ==, !=)
#define EXPECT_NEQ(x, y) EXPECT_OP(x, y, !=, ==)
#define EXPECT_GREATER(x, y) EXPECT_OP(x, y, >, <=)
#define EXPECT_LESS(x, y) EXPECT_OP(x, y, <, >=)
#define EXPECT_GREATER_OR_EQ(x, y) EXPECT_OP(x, y, >=, <)
#define EXPECT_LESS_OR_EQ(x, y) EXPECT_OP(x, y, <=, >)

#define ASSERT_OP(x, y, OPER, FAILURE)\
{\
	auto val_a = (x); auto val_b = (y);\
	if(!(val_a OPER val_b))\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("assertion not fulfilled");\
		printf(" -> |   Expected: "#x" "#OPER" "#y"\n");\
		printf(" -> |     Actual: %s", CLR_YELLOW);\
		YesTest::print_value(val_a);\
		printf(" " #FAILURE " ");\
		YesTest::print_value(val_b);\
		printf("%s\n", CLR_RESET);\
		return;\
	}\
}

#define ASSERT_EQ(x, y) ASSERT_OP(x, y, ==, !=)
#define ASSERT_NEQ(x, y) ASSERT_OP(x, y, !=, ==)
#define ASSERT_GREATER(x, y) ASSERT_OP(x, y, >, <=)
#define ASSERT_LESS(x, y) ASSERT_OP(x, y, <, >=)
#define ASSERT_GREATER_OR_EQ(x, y) ASSERT_OP(x, y, >=, <)
#define ASSERT_LESS_OR_EQ(x, y) ASSERT_OP(x, y, <=, >)

#define EXPECT_BETWEEN(x, low, high) {\
	auto val_a = (x); auto val_low = (low); auto val_high = (high);\
\
	if(val_a < val_low || val_a > val_high)\
	{\
		(*yes_test_errors) += 1;\
		YES_PRINT_FAILURE("expectation not fulfilled");\
		printf(" -> |   Expected: "#low" <= "#x" <= " #high"\n");\
		if(val_a < val_low)\
		{\
			printf(" -> |     Actual: %s", CLR_YELLOW);\
			YesTest::print_value(val_a);\
			printf(" < ");\
			YesTest::print_value(val_low);\
			printf("%s\n", CLR_RESET);\
		}\
		if(val_a > val_high)\
		{\
			printf(" -> |     Actual: %s", CLR_YELLOW);\
			YesTest::print_value(val_a);\
			printf(" > ");\
			YesTest::print_value(val_high);\
			printf("%s\n", CLR_RESET);\
		}\
	}\
}


// Each floating point operation has a error of up to 0.5 NUPs, so we use a default tolerance of 10 NUPs which should be enough for reasonable complex operations
#define EXPECT_EQf(x, y) EXPECT_SIMILAR(x, y, 10)
#define EXPECT_NEQf(x, y) EXPECT_UNSIMILAR(x, y, 10)