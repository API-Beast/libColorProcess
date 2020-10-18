#pragma once

#include "../Colors/ColorSpaces.h"
#include "ImageData.h"

namespace Image
{
	ImageData<sRGB_uint8> import_png_from_file(const char* filename);
	ImageData<sRGB_uint8> import_png_from_buffer(const unsigned char* buffer, int buffer_length);
	void export_image_data_to_png_file(const ImageData<sRGB_uint8>& data, const char* filename);
}