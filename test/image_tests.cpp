#include "yestest.h"
#include "test_utils.h"
#include <Image/ImageImport.h>
#include <sstream>

namespace
{
	ImageData<sRGB_uint8_Alpha> make_test_image(int w, int h)
	{
		ImageData<sRGB_uint8_Alpha> retVal;
		retVal.allocate(w, h);
		int i = 0;
		for(int y = 0; y < h; y++)
		for(int x = 0; x < w; x++)
		{
			// Try to make a pattern that is as easy as possible to figure out what is wrong exactly
			retVal.at(x, y) = sRGB_uint8_Alpha(x % 256, y % 256, std::pow(2, i++ % 32), std::pow(2, i++ % 32));
		}
		return retVal;
	}
}

YES_TEST(Image, copy)
{
	auto input = make_test_image(16, 16);
	auto copy = input;

	EXPECT_CONTAINER_EQ(copy, input);
}


YES_TEST(Image, tga_import_cycle)
{
	auto input = make_test_image(16, 16);
	std::stringstream buffer;
	Image::TGA::export_to_stream(input, buffer);
	EXPECT_TRUE(buffer.good());
	auto reimport = Image::TGA::import_from_stream(buffer);
	EXPECT_CONTAINER_EQ(reimport, input);
}

/*YES_TEST(Image, load_png_from_buffer)
{
	// Minimal PNG in hex
	const unsigned char transparentGreenPixel[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0x15, 0xc4, 0x89, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x44, 0x41, 0x54, 0x78, 0xda, 0x63, 0x64, 0xf8, 0xcf, 0xf0, 0x1f, 0x00, 0x04, 0x06, 0x02, 0x00, 0xa4, 0x3e, 0x5f, 0x44, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82};

	auto d = Image::import_png_from_buffer(transparentGreenPixel, sizeof(transparentGreenPixel)/sizeof(*transparentGreenPixel));
	ASSERT_EQ(d.size, Vec2i(1, 1));
	EXPECT_EQ(d[0].red,  0);
	EXPECT_EQ(d[0].green, 255);
	EXPECT_EQ(d[0].blue,  0);
	EXPECT_EQ(d[0].alpha,  255);
}
*/