#include <libColorTool.h>

int main()
{
	ImageData<sRGB_uint8> img = Image::TGA::import_from_file("source_image.tga").make_copy<sRGB_uint8>();
	// A 100x100 rectangle, with the left cornder at 154, 128 image coordinates
	for(auto& val : Iterate::rectangle(img, 154, 128, 100, 100))
		val = sRGB_uint8(255) - val;
	Image::TGA::export_to_file(img.make_copy<sRGB_uint8_Alpha>(), "invert_colors.tga");
}