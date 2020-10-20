#include "ImageImport.h"
#include "dependencies/spng.h"
#include <vector>
#include <fstream>
#include <cassert>
#include <cstdio>

namespace MinPNG
{
	#include "dependencies/minpng.h"
}

namespace Image
{
	ImageData<sRGB_uint8> import_png_from_file(const char* filename) 
	{
		std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
		std::ifstream::pos_type size = ifs.tellg();

		std::vector<unsigned char>  buffer(size);

		ifs.seekg(0, std::ios::beg);
		ifs.read(reinterpret_cast<char*>(buffer.data()), size);

		return import_png_from_buffer(buffer.data(), buffer.size());
	}

	ImageData<sRGB_uint8> import_png_from_buffer(const unsigned char* buffer, int buffer_length) 
	{
		ImageData<sRGB_uint8> img;
		unsigned error;

		spng_ctx* ctx = spng_ctx_new(0);

		spng_set_png_buffer(ctx, buffer, buffer_length);

		spng_ihdr header;
		spng_get_ihdr(ctx, &header);

		/* TODO: Handle color space data? */

		size_t size;
		spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &size);
		assert(size == header.width * header.height * sizeof(uint8_t)*3);

		img.allocate(header.width, header.height);
		spng_decode_image(ctx, img.data, img.data_length, SPNG_FMT_RGB8, 0);

		spng_ctx_free(ctx);
		return img;
	}
	
	void export_image_data_to_png_file(const ImageData<sRGB_uint8>& img, const char* filename) 
	{
		FILE *f = fopen(filename, "wb+");
		MinPNG::buf png = MinPNG::make_png(img.data, img.size.x, img.size.y, sizeof(sRGB_uint8), MinPNG::buf_cat_str_rgb);
		fwrite(png.data, png.len, 1, f);
		free(png.data);
		fclose(f);
	}
	
	// TODO: optimize copy away
	std::vector<unsigned char> export_image_data_to_png_buffer(const ImageData<sRGB_uint8>& img) 
	{
		MinPNG::buf png = MinPNG::make_png(img.data, img.size.x, img.size.y, sizeof(sRGB_uint8), MinPNG::buf_cat_str_rgb);
		std::vector<unsigned char> retVal(png.data, png.data+png.len);
		free(png.data);
		return retVal;
	}
}