#include "../Palette/Palette.h"
#include "../Palette/PaletteImport.h"
#include "test_utils.h"
#include "yestest.h"
#include <random>
#include <sstream>

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

	ColorPalette<sRGB_uint8> make_test_palette(int entries)
	{
		ColorPalette<sRGB_uint8> retVal;
		retVal.resize(entries, sRGB_uint8(255));
		for(int i = 0; i < entries; i++)
			retVal[i] = sRGB_uint8(i % 256, std::pow(2, i%9), std::pow(2, i%8+1));
		return retVal;
	};
}


YES_TEST(Palette, Conversion)
{
	ColorPalette<HCY> pal(512);
	std::generate(pal.begin(), pal.end(), random_color);
	ColorPalette<sRGB_uint8> conversion = Palette::convert<sRGB_uint8>(pal);

	EXPECT_EQ(conversion.size(), pal.size());
	EXPECT_EQ(conversion[0], colorspace_cast<sRGB_uint8>(pal[0]));
	EXPECT_EQ(conversion.back(), colorspace_cast<sRGB_uint8>(pal.back()));
}


YES_TEST(Palette, MedianSplit)
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

YES_TEST(Palette, MedianSplit_EdgeCases)
{
	// Edge case 1: All colors are the same or very similar: No axis to split
	sRGB baseColor = sRGB(0.5);
	ColorPalette<sRGB> pal(512, baseColor);
	ColorPalette<sRGB> reduced = Palette::reduce_using_median_split(pal, 32, Stats::perceptive_factors);
	EXPECT_EQ(pal.size(), 512);
	EXPECT_EQ(reduced.size(), 32);
	EXPECT_TRUE(std::all_of(reduced.begin(), reduced.end(), [baseColor](sRGB col){return col == baseColor; }));
}

YES_TEST(Palette, Sort)
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

YES_TEST(Palette, gpl_import_cycle)
{
	ColorPalette<sRGB_uint8> input = make_test_palette(16);
	std::stringstream buffer;
	Palette::GPL::export_to_stream(buffer, input);
	EXPECT_TRUE(buffer.good());
	auto reimport = Palette::GPL::import_from_stream(buffer);
	EXPECT_CONTAINER_EQ(reimport, input);
}