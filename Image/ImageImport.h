#pragma once

#include "../Colors/ColorSpaces.h"
#include "ImageData.h"

namespace Image
{
	ImageData<sRGBAu8> import_png_from_file(const char* filename);
	ImageData<sRGBAu8> import_png_from_buffer(const unsigned char* buffer, int buffer_length);
	void export_image_data_to_png_file(const ImageData<sRGBAu8>& data, const char* filename);
}