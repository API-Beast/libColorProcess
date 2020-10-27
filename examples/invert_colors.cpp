#include <libColorTool.h>

int main()
{
	ImageData<sRGB_uint8> img = Image::TGA::import_from_file("test_image.tga").make_copy<sRGB_uint8>();
	for(auto& val : img)
		val = sRGB_uint8(255) - val;
	Image::TGA::export_to_file(img.make_copy<sRGB_uint8_Alpha>(), "invert_colors_example.tga");
	printf("Result exported to invert_colors_example.tga");
}