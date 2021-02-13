#include <libColorProcess.h>

int main()
{
	ImageData<LinearRGB>    img        = Image::TGA::import_from_file("source_image.tga").make_copy<LinearRGB>();
	ColorPalette<LinearRGB> target_pal = Palette::convert<LinearRGB>(Palette::GPL::import_from_file("source_palette.gpl"));

	// With Iterate::chunks we can seperate the image into mutliple, same-sized parts.
	// These are of course only iterators, the image data itself is not affected.
	auto halves = Iterate::chunks(img, std::ceil(img.size.x/2.0), img.size.y);

	// This is the first way of mapping the colors, just comparing the color vectors directly, replacing each color with the palette entry whose distance is closest.
	// This is the method used in most image manipulation programs like GIMP or Photoshop, but is naive and yields low quality results.
	// We only apply it to the second half of the image.
	for(auto& color : halves[1])
		color = Palette::map_color_absolute(target_pal, color);
	
	Image::TGA::export_to_file("basic_remap.tga", img.make_copy<sRGB_uint8_Alpha>());
}