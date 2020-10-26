#pragma once

#include "../Colors/ColorSpaces.h"
#include "ImageData.h"
#include <vector>

namespace Image
{
	ImageData<sRGB_uint8> import_png_from_file(const char* filename);
	ImageData<sRGB_uint8> import_png_from_buffer(const unsigned char* buffer, int buffer_length);
	void export_image_data_to_png_file(const ImageData<sRGB_uint8>& data, const char* filename);
	std::vector<unsigned char> export_image_data_to_png_buffer(const ImageData<sRGB_uint8>& data);
	void export_image_data_to_tga_file(const ImageData<sRGB_uint8_Alpha>& data, const char* filename);

	/*
	namespace PNG
	{
		ImageData<sRGB_Alpha_uint8> import_file(const char* filename);
		ImageData<sRGB_Alpha_uint8> import_buffer(std::istream input);
		void export_to_file(const ImageData<sRGB_Alpha_uint8>& data, const char* filename);
		void export_to_buffer(const ImageData<sRGB_Alpha_uint8>& data, std::ostream output);
	}

	namespace TGA
	{
		ImageData<sRGB_Alpha_uint8> import_file(const char* filename);
		ImageData<sRGB_Alpha_uint8> import_buffer(std::istream input);
		void export_to_file(const ImageData<sRGB_Alpha_uint8>& data, const char* filename);
		void export_to_buffer(const ImageData<sRGB_Alpha_uint8>& data, std::ostream output);
	}
	*/
}