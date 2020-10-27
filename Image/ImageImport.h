#pragma once

#include "../Colors/ColorSpaces.h"
#include "ImageData.h"
#include <vector>

namespace Image
{
	/*
	namespace PNG
	{
		ImageData<sRGB_uint8_Alpha> import_file(const char* filename);
		ImageData<sRGB_uint8_Alpha> import_buffer(std::istream input);
		void export_to_file(const ImageData<sRGB_uint8_Alpha>& data, const char* filename);
		void export_to_buffer(const ImageData<sRGB_uint8_Alpha>& data, std::ostream output);
	}*/

	namespace TGA
	{
		ImageData<sRGB_uint8_Alpha> import_from_file(const char* filename);
		ImageData<sRGB_uint8_Alpha> import_from_stream(std::istream& input);
		void export_to_file(const ImageData<sRGB_uint8_Alpha>& data, const char* filename);
		void export_to_stream(const ImageData<sRGB_uint8_Alpha>& data, std::ostream& output);
	}
}