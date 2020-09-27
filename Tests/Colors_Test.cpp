#include "utest.h"
#include <ColorToolCore.h>
#include <type_traits>

UTEST_MAIN();

UTEST(Colors, ImplicitConversion)
{
	using namespace Colors;
	LinRGB a(0.5, 0.5, 0.5);
	sRGB b = a;
	LinRGB c = b;
	EXPECT_EQ(luminance709(a), luminance709(b));
	EXPECT_EQ(luminance709(a), luminance709(c));
	static_assert(std::is_same<decltype(a+b), decltype(a)>::value, "Operators have wrong result type");
}