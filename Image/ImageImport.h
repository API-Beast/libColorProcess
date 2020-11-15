#pragma once

#include "../Colors/ColorSpaces.h"
#include "ImageData.h"
#include <vector>

namespace Image
{
	namespace TGA
	{
		// If input stream or image is invalid, will return ImageData::fallback_image (a single white pixel with invalid flag = true)
		ImageData<sRGB_uint8_Alpha> import_from_file(const char* filename);
		ImageData<sRGB_uint8_Alpha> import_from_stream(std::istream& input);
		void export_to_file(const char* filename, const ImageData<sRGB_uint8_Alpha>& data);
		void export_to_stream(std::ostream& output, const ImageData<sRGB_uint8_Alpha>& data);
	}
}