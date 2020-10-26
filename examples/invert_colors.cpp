#include <libColorTool.h>

int main()
{
	ImageData<sRGB_uint8> img = Image::import_png_from_file("test_image.png");
	//ImageIterator half_image = img.rectangle(img.size.x/2, 0, img.size.x/2, img.size.y);
	//Filter::invert(half_image.begin(), half_image.end(), half_image.begin());
	std::transform(img.begin(), img.end(), img.begin(), [](auto val)->sRGB_uint8{ return sRGB_uint8(0, val.green, val.blue); });
	Image::export_image_data_to_tga_file(img.make_copy<sRGB_uint8_Alpha>(), "invert_colors_example.tga");
	printf("Result exported to invert_colors_example.tga");
}