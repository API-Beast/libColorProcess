#include "yestest.h"
#include <ColorToolCore.h>
#include <type_traits>
#include <random>

YES_MAIN();

using namespace Colors;

namespace YesTest
{
	template<typename T, auto X, auto Y, auto Z>
	void print_value(Vector3Mixin<T,X,Y,Z> f) { printf("%f %f %f", double(f.*X), double(f.*Y), double(f.*Z));}

	template<typename T, auto X, auto Y, auto Z>
	inline int ulp_difference(Vector3Mixin<T,X,Y,Z> a, Vector3Mixin<T,X,Y,Z> b)
	{
		return std::max({ulp_difference(a.*X, b.*X), ulp_difference(a.*Y, b.*Y), ulp_difference(a.*Z, b.*Z)});
	}
}

namespace
{
	HCY random_color()
	{
		static std::random_device rd;
		static std::default_random_engine rng(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		
		HCY retVal;
		retVal.hue = dist(rng);
		retVal.chroma = dist(rng);

		auto limits = HCY::get_luminance_limits(retVal.hue, retVal.chroma);
		std::uniform_real_distribution<double> lum_dist(limits.first, limits.second);
		retVal.luminance = lum_dist(rng);

		return retVal;
	};
}

YES_TEST(Colors, colorspace_cast)
{
	//EXPECT_EQ(colorspace_cast<LinRGB>(HCY(0.0, 1.0, luminance709(LinRGB(1.0, 0.0, 0.0)))), LinRGB(1.0, 0.0, 0.0));
	EXPECT_EQ(colorspace_cast<sRGB>(sRGBu8(255, 255, 255)), sRGB(1.0, 1.0, 1.0));
	EXPECT_EQ(colorspace_cast<LinRGB>(sRGBu8(255, 255, 255)), LinRGB(1.0, 1.0, 1.0));
}

YES_TEST(Colors, ColorPalette)
{
	ColorPalette<HCY> pal;
	pal.entries.resize(512);
	std::generate(pal.entries.begin(), pal.entries.end(), random_color);

	ColorPalette<HCY> copy = pal;
	copy.reduce_using_median_split(32, calc_perceptive_factors);
	EXPECT_EQ(pal.size(), 512);
	EXPECT_EQ(copy.size(), 32);

	int entries_in_both = 0;
	for(HCY val : copy.entries)
	{
		auto where = std::find(pal.entries.begin(), pal.entries.end(), val);
		if(where != pal.entries.end())
			entries_in_both++;
	}
	EXPECT_EQ(entries_in_both, copy.size());

	ColorPalette<sRGBu8> conversion = copy;
	EXPECT_EQ(conversion.size(), copy.size());
	EXPECT_EQ(conversion.entries[0], colorspace_cast<sRGBu8>(copy.entries[0]));
	EXPECT_EQ(conversion.entries.back(), colorspace_cast<sRGBu8>(copy.entries.back()));
}

YES_TEST(Colors, Functions)
{
	EXPECT_EQf(saturate_hue(0.000000), Vec3f(1.0f, 0.0f, 0.0f));
	EXPECT_EQf(saturate_hue(0.333333), Vec3f(0.0f, 1.0f, 0.0f));
	EXPECT_EQf(saturate_hue(0.666666), Vec3f(0.0f, 0.0f, 1.0f));
}

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

YES_TEST(Colors, ImplicitConversion)
{
	LinRGB a(0.5, 0.5, 0.5);
	sRGB b = a;
	LinRGB c = b;
	EXPECT_EQf(luminance709(a), luminance709(b));
	EXPECT_EQf(luminance709(a), luminance709(c));
	EXPECT_EQf((a+b).red, a.red + sRGB::sRGB_component_to_linear_component(b.red));
	static_assert(std::is_same<decltype(a+b), decltype(a)>::value, "Operators have wrong result type");
}

YES_TEST(Colors, sRGBu8)
{
	sRGB f(0.0f, 0.5f, 1.0f);
	sRGBu8 u8 = f;
	EXPECT_EQ(u8.red,     0);
	EXPECT_EQ(u8.green, 127);
	EXPECT_EQ(u8.blue,  255);

	sRGB inv = u8;
	EXPECT_EQ(inv.red,   f.red);
	EXPECT_BETWEEN(inv.green, 0.49, 0.51);
	EXPECT_EQ(inv.blue,  f.blue);
}

YES_TEST(Colors, HSV_Conversions)
{
	sRGB d(0.5, 0.5, 0.5);
	sHSV e = d;
	sRGB f = e;
	EXPECT_EQ(d, f);
	PRINT_INFO(e);

	LinRGB a(0.5, 0.5, 0.5);
	LinHSV b = a;
	LinRGB c = b;
	EXPECT_EQ(a, c);
	PRINT_INFO(b);
}

YES_TEST(Colors, HSV)
{
	LinRGB red(1.0, 0.0, 0.0);
	LinHSV red_hsv(red);
	EXPECT_EQ(red_hsv.hue, 0.0f);
	EXPECT_EQ(red_hsv.saturation, 1.0f);
	EXPECT_EQf(luminance709(red_hsv), luminance709(red));
	EXPECT_EQf(LinRGB(red_hsv), red);

	LinRGB green(0.0, 1.0, 0.0);
	LinHSV green_hsv(green);
	EXPECT_EQf(green_hsv.hue, 1.0f / 3.0f);
	EXPECT_EQ(green_hsv.saturation, 1.0f);
	EXPECT_EQf(luminance709(green_hsv), luminance709(green));
	EXPECT_EQf(LinRGB(green_hsv), green);

	LinRGB blue(0.0, 0.0, 1.0);
	LinHSV blue_hsv(blue);
	EXPECT_EQf(blue_hsv.hue, 2.0f / 3.0f);
	EXPECT_EQ(blue_hsv.saturation, 1.0f);
	EXPECT_EQf(luminance709(blue_hsv), luminance709(blue));
	EXPECT_EQf(LinRGB(blue_hsv), blue);
}

YES_TEST(Colors, LinRGB_to_HCY)
{
	LinRGB red(1.0, 0.0, 0.0);
	HCY red_hcy(red);
	EXPECT_EQ(red_hcy.hue, 0.0f);
	EXPECT_EQ(red_hcy.chroma, 1.0f);
	EXPECT_EQf(red_hcy.luminance, luminance709(red));

	LinRGB green(0.0, 1.0, 0.0);
	HCY green_hcy(green);
	EXPECT_EQf(green_hcy.hue, 1.0f / 3.0f);
	EXPECT_EQ(green_hcy.chroma, 1.0f);
	EXPECT_EQf(green_hcy.luminance, luminance709(green));

	LinRGB blue(0.0, 0.0, 1.0);
	HCY blue_hcy(blue);
	EXPECT_EQf(blue_hcy.hue, 2.0f / 3.0f);
	EXPECT_EQ(blue_hcy.chroma, 1.0f);
	EXPECT_EQf(blue_hcy.luminance, luminance709(blue));
}

YES_TEST(Colors, HCY_to_LinRGB)
{
	EXPECT_EQ(colorspace_cast<LinRGB>(HCY(0.0, 1.0, luminance709(LinRGB(1.0, 0.0, 0.0)))), LinRGB(1.0, 0.0, 0.0));
}