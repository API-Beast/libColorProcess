#include "yestest.h"
#include <libColorTool.h>
#include <type_traits>
#include <random>
#include "test_utils.h"

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

YES_TEST(Colors, Palette_MedianSplit)
{
	ColorPalette<HCY> pal;
	pal.resize(512);
	std::generate(pal.begin(), pal.end(), random_color);

	ColorPalette<HCY> reduced = Palette::reduce_using_median_split(pal, 32, Stats::perceptive_factors);
	EXPECT_EQ(pal.size(), 512);
	EXPECT_EQ(reduced.size(), 32);

	int entries_in_both = 0;
	for(HCY val : reduced)
	{
		auto where = std::find(pal.begin(), pal.end(), val);
		if(where != pal.end())
			entries_in_both++;
	}
	EXPECT_EQ(entries_in_both, reduced.size());
}

YES_TEST(Colors, Palette_MedianSplit_EdgeCases)
{
	// Edge case 1: All colors are the same or very similar: No axis to split
	sRGB baseColor = sRGB(0.5);
	ColorPalette<sRGB> pal(512, baseColor);
	ColorPalette<sRGB> reduced = Palette::reduce_using_median_split(pal, 32, Stats::perceptive_factors);
	EXPECT_EQ(pal.size(), 512);
	EXPECT_EQ(reduced.size(), 32);
	EXPECT_TRUE(std::all_of(reduced.begin(), reduced.end(), [baseColor](sRGB col){return col == baseColor; }));
}

YES_TEST(Colors, Palette_Conversion)
{
	ColorPalette<HCY> pal(512);
	std::generate(pal.begin(), pal.end(), random_color);
	ColorPalette<sRGB_uint8> conversion = Palette::convert<sRGB_uint8>(pal);

	EXPECT_EQ(conversion.size(), pal.size());
	EXPECT_EQ(conversion[0], colorspace_cast<sRGB_uint8>(pal[0]));
	EXPECT_EQ(conversion.back(), colorspace_cast<sRGB_uint8>(pal.back()));
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

YES_TEST(Colors, Palette_Sort)
{
	ColorPalette<HCY> pal(512);
	std::generate(pal.begin(), pal.end(), random_color);

	ColorPalette<HCY> sorted = Palette::sort(pal, Stats::linrgb_factors);
	EXPECT_EQ(pal.size(), 512);
	EXPECT_EQ(sorted.size(), 512);
	ASSERT_EQ(pal.size(), sorted.size());

	int equal_entries = 0;
	int entries_in_both = 0;
	for(int i = 0; i<sorted.size(); i++)
	{
		if(pal[i] == sorted[i])
			equal_entries++;
		auto where = std::find(pal.begin(), pal.end(), sorted[i]);
		if(where != pal.end())
			entries_in_both++;
	}
	EXPECT_LESS(equal_entries, sorted.size()/10); // Extremely unlikely that more than 10% of entries end up randomly pre-sorted
	EXPECT_EQ(entries_in_both, sorted.size());
}

YES_TEST(Colors, Functions)
{
	EXPECT_EQf(Colors::saturate_hue(0.000000), Vec3f(1.0f, 0.0f, 0.0f));
	EXPECT_EQf(Colors::saturate_hue(0.333333), Vec3f(0.0f, 1.0f, 0.0f));
	EXPECT_EQf(Colors::saturate_hue(0.666666), Vec3f(0.0f, 0.0f, 1.0f));
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