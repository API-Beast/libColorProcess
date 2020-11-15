#include <libColorTool.h>

int main()
{
	ImageData<LinearRGB>    img     = Image::TGA::import_from_file("source_image.tga").make_copy<LinearRGB>();
	ColorPalette<LinearRGB> target_pal = Palette::convert<LinearRGB>(Palette::GPL::import_from_file("source_palette.gpl"));

	// The first way we can improve this is by using different set of comparison factors from the raw color values.
	// Then we can find the minimum and maximum of each of these comparison factors,
	// this enables us to take the distance of the normalized factors rather than of the absolute factors.
	auto img_stats    = Stats::gather_stat_range(img.begin(),        img.end(),        Stats::perceptive_factors);
	auto target_stats = Stats::gather_stat_range(target_pal.begin(), target_pal.end(), Stats::perceptive_factors);
	for(auto& color : img)
		color = Palette::map_color_relative(target_pal, color, Stats::perceptive_factors, target_stats, img_stats, {6.0, 1.0, 1.0, 1.0, 1.0});
	
	Image::TGA::export_to_file("smart_remap.tga", img.make_copy<sRGB_uint8_Alpha>());
}