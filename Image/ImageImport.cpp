#include "ImageImport.h"
#include "dependencies/spng.h"
#include <vector>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <new>

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
		unsigned char* imported_data = new unsigned char[size];

		spng_decode_image(ctx, imported_data, size, SPNG_FMT_RGB8, 0);
		// Are you drunk SPNG? The returned data is BGR not RGB
		img.import_c_array<unsigned char, sRGB_uint8, 3, 2, 1, 0>(header.width, header.height, imported_data, size);

		spng_ctx_free(ctx);
		delete[] imported_data;
		return img;
	}
	
	void export_image_data_to_png_file(const ImageData<sRGB_uint8>& img, const char* filename) 
	{
		std::FILE *f = std::fopen(filename, "wb+");
		MinPNG::buf png = MinPNG::make_png(img.data, img.size.x, img.size.y, sizeof(sRGB_uint8), MinPNG::buf_cat_str_rgb);
		std::fwrite(png.data, png.len, 1, f);
		std::free(png.data);
		std::fclose(f);
	}
	
	std::vector<unsigned char> export_image_data_to_png_buffer(const ImageData<sRGB_uint8>& img) 
	{
		MinPNG::buf png = MinPNG::make_png(img.data, img.size.x, img.size.y, sizeof(sRGB_uint8), MinPNG::buf_cat_str_rgb);
		std::vector<unsigned char> retVal(png.data, png.data+png.len);
		std::free(png.data);
		return retVal;
	}
	
	namespace TGA
	{
		#pragma scalar_storage_order little_endian
		#pragma pack(push, 1)
		struct Header
		{
			uint8_t id_length = 0;
			uint8_t color_map_type = 0;
			uint8_t image_type = 2; // Uncompressed RGB image
			// Color Map
			uint16_t first_index = 0;
			uint16_t number_of_entries = 0;
			uint8_t color_map_bits_per_pixel = 0;
			// Image Description
			uint16_t x_origin = 0;
			uint16_t y_origin = 0;
			uint16_t width  = 0;
			uint16_t height = 0;
			uint8_t image_bits_per_pixel = 32;
			uint8_t bits_per_component:3,
					reserved:1,
					screen_origin:1,
					interleaving_mode:2;
		};
		#pragma pack(pop)
		#pragma scalar_storage_order default
	}

	void export_image_data_to_tga_file(const ImageData<sRGB_uint8_Alpha>& data, const char* filename) 
	{
		TGA::Header hdr;
		hdr.width = data.size.x;
		hdr.height = data.size.y;
		hdr.bits_per_component = 0;
		hdr.reserved = 0;
		hdr.screen_origin = 0;
		hdr.interleaving_mode = 0;

		FILE *f = std::fopen(filename, "wb+");
		std::fwrite(&hdr, 1, sizeof(hdr), f);
		// 0 size image identification
		// 0 size color map data
		for(auto& color : data)
		{
			// Endian weirdness, TGA seems to have stored the color as integer, inheriting the endianness
			uint8_t buffer[4] = {color.blue, color.red, color.green, color.alpha};
			std::fwrite(buffer, sizeof(uint8_t), 4, f);
		}
	}
}