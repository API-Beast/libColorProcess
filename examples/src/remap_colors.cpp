#include <libColorTool.h>

int main()
{
	ImageData<LinearRGB>    img     = Image::TGA::import_from_file("source_image.tga").make_copy<LinearRGB>();
	ColorPalette<LinearRGB> img_pal = Palette::import_from_image(img);
	ColorPalette<LinearRGB> target_pal = Palette::convert<LinearRGB>(Palette::import_from_gpl_file("source_palette.gpl"));

	// We split the image into 4 parts, so we can apply a different algorithm to each one.
	auto quadrants = Iterate::chunks(img, std::ceil(img.size.x/2.0), std::ceil(img.size.y/2.0));

	// This is the first way of mapping the colors, just comparing the color vectors directly, replacing each color with the palette entry whose distance is closest.
	// This is the method used in most image manipulation programs like GIMP or Photoshop, but is naive and yields low quality results.
	for(auto& color : quadrants[1])
		color = Palette::map_color_absolute(target_pal, color);

	// The first way we can improve this is by using different set of comparison factors from the raw color values.
	// Then we can find the minimum and maximum of each of these comparison factors,
	// this enables us to take the distance of the normalized factors rather than of the absolute factors.
	auto img_stats    = Palette::gather_stat_range(img_pal,    &Stats::perceptive_factors);
	auto target_stats = Palette::gather_stat_range(target_pal, &Stats::perceptive_factors);
	for(auto& color : quadrants[2])
		color = Palette::map_color_relative(target_pal, color, &Stats::perceptive_factors, target_stats, img_stats);
	
	Image::TGA::export_to_file(img.make_copy<sRGB_uint8_Alpha>(), "remap_colors.tga");
}