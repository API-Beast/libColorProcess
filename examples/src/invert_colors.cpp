#include <libColorProcess.h>

int main()
{
	ImageData<sRGB_uint8> img = Image::TGA::import_from_file("source_image.tga").make_copy<sRGB_uint8>();
	// Apply to a 50x200 rectangle, with the top left corner at 170, 11 image coordinates
	for(auto& val : Iterate::rectangle(img, 170, 11, 50, 200))
		val = sRGB_uint8(255) - val;
	Image::TGA::export_to_file("invert_colors.tga", img.make_copy<sRGB_uint8_Alpha>());
}