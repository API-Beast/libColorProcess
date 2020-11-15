#include "yestest.h"
#include "test_utils.h"
#include <Image/ImageImport.h>
#include <Image/ImageIterator.h>
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
			retVal.at(x, y) = sRGB_uint8_Alpha(x % 256, y % 256, int(std::pow(2, i % 8)) % 256, int(std::pow(2, i+1 % 8)) % 256);
			i++;
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
	Image::TGA::export_to_stream(buffer, input);
	EXPECT_TRUE(buffer.good());
	auto reimport = Image::TGA::import_from_stream(buffer);
	EXPECT_CONTAINER_EQ(reimport, input);
}

YES_TEST(ImageIterator, internal_logic)
{
	ImageData<char> dummy;
	dummy.data = 0x0;
	dummy.size = Vec2i(10, 10);
	dummy.data_length = 100;

	auto region = Iterate::region(dummy, 1, 1, 4, 4);
	
	EXPECT_EQ(size_t(region.start.ptr), 11);
	EXPECT_EQ(region.start.width, 3);
	EXPECT_EQ(region.start.row_offset, 10);
	EXPECT_EQ(size_t(region.start.row_start), 11);

	EXPECT_EQ(size_t(region.stop.ptr), 41);
	EXPECT_EQ(region.stop.width, 3);
	EXPECT_EQ(region.stop.row_offset, 10);
	EXPECT_EQ(size_t(region.stop.row_start), 41);

	std::vector<size_t> visited;
	std::vector<size_t> expected({11, 12, 13, 21, 22, 23, 31, 32, 33});
	auto it = region.begin();
	while(it != region.end())
	{
		visited.push_back(size_t(it.ptr));
		it++;
	}
	EXPECT_CONTAINER_EQ(visited, expected);
}

YES_TEST(ImageIterator, operator_plus)
{	
	auto input = make_test_image(16, 16);
	auto rect_10_10 = Iterate::rectangle(input, 0, 0, 10, 10);
	auto it = rect_10_10.begin();
	EXPECT_EQ(&(*it), &input[0]);
	EXPECT_EQ(&(*++it), &input[1]);
	EXPECT_EQ(&(*++it), &input[2]);

	it = rect_10_10.begin();
	it += 10;
	EXPECT_EQ(&(*it), input.row(1));

	it = rect_10_10.begin();
	auto increased = it + 10;
	EXPECT_EQ(increased - it, 10);

	it = rect_10_10.begin();
	it += 40;
	EXPECT_EQ(&(*it), input.row(4));
}

YES_TEST(ImageIterator, x_offset)
{
	auto input = make_test_image(16, 16);
	for(int x = 0; x<4; x++)
	{
		auto rect = Iterate::rectangle(input, x, 0, 4, 4);
		auto it = rect.begin();
		EXPECT_EQ(&*(it) - input.begin(),   &(input.at(x, 0)) - input.begin());
		EXPECT_EQ(&*(it+4) - input.begin(), &(input.at(x, 1)) - input.begin());
		PRINT_INFO(it.ptr);
		PRINT_INFO(it.row_start);
		PRINT_INFO(it.row_offset);
	}
}

YES_TEST(ImageIterator, y_offset)
{	
	auto input = make_test_image(16, 16);
	for(int y = 0; y<4; y++)
	{
		auto rect = Iterate::rectangle(input, 0, y, 4, 4);
		auto it = rect.begin();
		EXPECT_EQ(&*(it), &(input.at(0, y)));
		EXPECT_EQ(&*(it+4), &(input.at(0, y+1)));
	}
}

YES_TEST(ImageIterator, iterate)
{
	int w = 2;
	int h = 2;
	int x_off = 8;
	int y_off = 8;
	auto input = make_test_image(32, 32);

	// With offset
	auto rect = Iterate::rectangle(input, x_off, y_off, w, h);
	int x = 0;
	int y = 0;
	for(auto it = rect.begin(); it != rect.end(); it++)
	{
		EXPECT_EQ(&(*it), &(input.at(x + x_off, y + y_off)));
		x++;
		if(x >= w)
		{
			x = 0;
			y++;
		}
	}

	// Without offset
	rect = Iterate::rectangle(input, 0, 0, w, h);
	x = 0;
	y = 0;
	for(auto it = rect.begin(); it != rect.end(); it++)
	{
		EXPECT_EQ(&(*it), &(input.at(x, y)));
		x++;
		if(x >= w)
		{
			x = 0;
			y++;
		}
	}
}

YES_TEST(ImageIterator, copy)
{
	auto input = make_test_image(24, 16);

	auto rect_a = Iterate::rectangle(input, 0, 0, input.size.x / 2, input.size.y);
	auto rect_b = Iterate::rectangle(input, input.size.x / 2, 0, input.size.x / 2, input.size.y);
	std::copy(rect_a.begin(), rect_a.end(), rect_b.begin());
	EXPECT_CONTAINER_EQ(rect_a, rect_b);
}

YES_TEST(ImageIterator, oversized)
{
	auto input = make_test_image(100, 100);

	auto oversized = Iterate::rectangle(input, -64, -64, 100 + 64 + 100, 100 + 64 + 100);
	auto exact = Iterate::rectangle(input, 0, 0, 100, 100);
	EXPECT_EQ(&*oversized.begin() - &*input.begin(), 0);
	EXPECT_EQ(&*exact.begin() - &*input.begin(), 0);
	EXPECT_EQ(exact.begin() - oversized.begin(), 0);
	EXPECT_EQ(exact.end() - oversized.end(), 0);

	EXPECT_EQ(&*(exact.end() - 1) - &*(input.end()-1), 0);
}

YES_TEST(ImageIterator, undersized)
{
	auto input = make_test_image(100, 100);
	auto undersized_x = Iterate::region(input, 120, 0, 130, 10);
	EXPECT_EQ(undersized_x.end() - undersized_x.begin(), 0);

	auto undersized_y = Iterate::region(input, 0, -20, 10, -10);
	EXPECT_EQ(undersized_y.end() - undersized_y.begin(), 0);
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