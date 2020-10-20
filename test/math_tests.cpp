#include "../Math/Vector3.h"
#include "test_utils.h"

YES_TEST(Math, Vec3f)
{
	Vec3f a{0.1, 0.2, 0.3};
	Vec3f b(0.1, 0.2, 0.3);
	Vec3f c = Vec3f(0.1, 0.2, 0.3);
	EXPECT_EQf(a.x, 0.1);
	EXPECT_EQf(a.y, 0.2);
	EXPECT_EQf(a.z, 0.3);
	EXPECT_EQ(a, b);
	EXPECT_EQ(b, c);
	PRINT_INFO(a);
	PRINT_INFO(b);
	PRINT_INFO(c);

	float value = 1.0;
	float saturation = 0.0;
	Vec3f gray = Vec3f(1.0f);
	gray *= value;
	gray -= value * saturation;
	EXPECT_EQ(gray, Vec3f(1.0));
	EXPECT_EQ(Vec3f(1.0) * value - (value * saturation), Vec3f(1.0));
	Vec3f saturated = {1.0, 0.0, 0.0};
	Vec3f sum = saturated * value * saturation + gray;
	EXPECT_EQ(sum, Vec3f(1.0));
}