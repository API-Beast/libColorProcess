#include "yestest.h"
#include <libColorTool.h>
#include <type_traits>
#include <random>
#include "test_utils.h"

YES_TEST(Colors, colorspace_cast)
{
	LinearRGB src(0.0, 0.0, 1.0);
	sRGB srgb = colorspace_cast<sRGB>(src);
	sRGB_uint8 srgb8 = colorspace_cast<sRGB_uint8>(src);
	HSV shsv = colorspace_cast<HSV>(src);
	LinearHSV linhsv = colorspace_cast<LinearHSV>(src);
	HCY hcy = colorspace_cast<HCY>(src);

	EXPECT_EQf(colorspace_cast<LinearRGB>(srgb), src);
	EXPECT_EQf(colorspace_cast<LinearRGB>(srgb8), src);
	EXPECT_EQf(colorspace_cast<LinearRGB>(shsv), src);
	EXPECT_EQf(colorspace_cast<LinearRGB>(linhsv), src);
	EXPECT_EQf(colorspace_cast<LinearRGB>(hcy), src);
	PRINT_INFO(srgb);
	PRINT_INFO(srgb8);
	PRINT_INFO(shsv);
	PRINT_INFO(linhsv);
	PRINT_INFO(hcy);
}

YES_TEST(Colors, GenericColor)
{
	GenericColor color(LinearRGB(1.0));
	EXPECT_EQf(sRGB(color), sRGB(1.0));
	EXPECT_EQf(sRGB_uint8(color), sRGB_uint8(255));
	EXPECT_EQf(LinearRGB(color), LinearRGB(1.0));
	EXPECT_EQf(HSV(color), HSV(0.0, 0.0, 1.0));
	color.convert_to(ColorSpace::sRGB_uint8);
	EXPECT_EQf(color.get_vector(), Vec3f(255.0));
	EXPECT_EQf(sRGB(color), sRGB(1.0));
	EXPECT_EQf(sRGB_uint8(color), sRGB_uint8(255));
}

YES_TEST(Colors, Functions)
{
	EXPECT_EQf(Colors::saturate_hue(0.000000), Vec3f(1.0f, 0.0f, 0.0f));
	EXPECT_EQf(Colors::saturate_hue(0.333333), Vec3f(0.0f, 1.0f, 0.0f));
	EXPECT_EQf(Colors::saturate_hue(0.666666), Vec3f(0.0f, 0.0f, 1.0f));
}


YES_TEST(Colors, ImplicitConversion)
{
	LinearRGB a(0.5, 0.5, 0.5);
	sRGB b = a;
	LinearRGB c = b;
	EXPECT_EQf(Colors::luminance709(a), Colors::luminance709(b));
	EXPECT_EQf(Colors::luminance709(a), Colors::luminance709(c));
	//EXPECT_EQf((a+b).red, a.red + Colors::component_sRGB_to_linear(b.red));
	//static_assert(std::is_same<decltype(a+b), decltype(a)>::value, "Operators have wrong result type");
}

YES_TEST(Colors, sRGB_uint8)
{
	sRGB f(0.0f, 0.5f, 1.0f);
	sRGB_uint8 u8 = f;
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
	HSV e = d;
	sRGB f = e;
	EXPECT_EQ(d, f);
	PRINT_INFO(e);

	LinearRGB a(0.5, 0.5, 0.5);
	LinearHSV b = a;
	LinearRGB c = b;
	EXPECT_EQ(a, c);
	PRINT_INFO(b);
}

YES_TEST(Colors, HSV)
{
	LinearRGB red(1.0, 0.0, 0.0);
	LinearHSV red_hsv(red);
	EXPECT_EQ(red_hsv.hue, 0.0f);
	EXPECT_EQ(red_hsv.saturation, 1.0f);
	EXPECT_EQf(Colors::luminance709(red_hsv), Colors::luminance709(red));
	EXPECT_EQf(LinearRGB(red_hsv), red);

	LinearRGB green(0.0, 1.0, 0.0);
	LinearHSV green_hsv(green);
	EXPECT_EQf(green_hsv.hue, 1.0f / 3.0f);
	EXPECT_EQ(green_hsv.saturation, 1.0f);
	EXPECT_EQf(Colors::luminance709(green_hsv), Colors::luminance709(green));
	EXPECT_EQf(LinearRGB(green_hsv), green);

	LinearRGB blue(0.0, 0.0, 1.0);
	LinearHSV blue_hsv(blue);
	EXPECT_EQf(blue_hsv.hue, 2.0f / 3.0f);
	EXPECT_EQ(blue_hsv.saturation, 1.0f);
	EXPECT_EQf(Colors::luminance709(blue_hsv), Colors::luminance709(blue));
	EXPECT_EQf(LinearRGB(blue_hsv), blue);
}

YES_TEST(Colors, LinRGB_to_HCY)
{
	LinearRGB red(1.0, 0.0, 0.0);
	HCY red_hcy(red);
	EXPECT_EQ(red_hcy.hue, 0.0f);
	EXPECT_EQ(red_hcy.chroma, 1.0f);
	EXPECT_EQf(red_hcy.luminance, Colors::luminance709(red));

	LinearRGB green(0.0, 1.0, 0.0);
	HCY green_hcy(green);
	EXPECT_EQf(green_hcy.hue, 1.0f / 3.0f);
	EXPECT_EQ(green_hcy.chroma, 1.0f);
	EXPECT_EQf(green_hcy.luminance, Colors::luminance709(green));

	LinearRGB blue(0.0, 0.0, 1.0);
	HCY blue_hcy(blue);
	EXPECT_EQf(blue_hcy.hue, 2.0f / 3.0f);
	EXPECT_EQ(blue_hcy.chroma, 1.0f);
	EXPECT_EQf(blue_hcy.luminance, Colors::luminance709(blue));
}

YES_TEST(Colors, HCY_to_LinRGB)
{
	EXPECT_EQf(colorspace_cast<LinearRGB>(HCY(0.0, 1.0, Colors::luminance709(LinearRGB(1.0, 0.0, 0.0)))), LinearRGB(1.0, 0.0, 0.0));
}