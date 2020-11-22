#include <libColorTool.h>

int main(int argc, const char** argv)
{
	const char* input_image = "source_image.tga";
	const char* output_image = "smart_remap.tga";
	if(argc > 2)
	{
		input_image = argv[1];
		output_image = argv[2];
	}

	ImageData<LinearRGB>    img        = Image::TGA::import_from_file(input_image).make_copy<LinearRGB>();
	ColorPalette<LinearRGB> target_pal = Palette::convert<LinearRGB>(Palette::GPL::import_from_file("source_palette.gpl"));

	// The first way we can improve this is by using different set of comparison factors from the raw color values.
	// Then we can find the minimum and maximum of each of these comparison factors,
	// this enables us to take the distance of the normalized factors rather than of the absolute factors.
	auto img_stats    = Stats::gather_stat_range(img.begin(),        img.end(),        Stats::perceptive_factors);
	auto target_stats = Stats::gather_stat_range(target_pal.begin(), target_pal.end(), Stats::perceptive_factors);
	// Further, we can give these values different weights to come closer to human perception.
	// Humans are much more sensitive to Luminance/Brightness than everything else, so we give it 3 times the weight.
	std::array<float, 5> weights = {6.0f, 1.0f, 1.0f, 1.0f, 1.0f}; 

	// With these parameters set up we can do "relative" color mapping.
	// Finally we can interpolate between multiple samples in order to get a result with smooth transitions.
	for(auto& color : img)
		color = Palette::map_color_relative_multisample(target_pal, color, Stats::perceptive_factors, target_stats, img_stats, weights);
	
	Image::TGA::export_to_file(output_image, img.make_copy<sRGB_uint8_Alpha>());
}