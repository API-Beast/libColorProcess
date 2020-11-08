#include "Palette.h"
#include <fstream>

ColorPalette<sRGB_uint8> Palette::import_from_gpl_file(const char* filename)
{
	std::fstream input(filename, std::ios_base::in);
	ColorPalette<sRGB_uint8> palette;
	input.ignore(1024, '\n'); // # GIMP Palette or KOffice Palette
	input.ignore(1);
	while(!input.eof())
	{
		input >> std::ws;
		if(input.peek() == '#') // Line comments
			continue;
		if(input.peek() == '\n') // Empty lines
			continue;

		Vector3<int32_t> color;
		input >> std::skipws;
		input >> color.x >> color.y >> color.z;
		input >> std::noskipws;
		input.ignore(1024, '\n'); // Ignore the rest of the line
		input.ignore(1);
		palette.push_back(sRGB_uint8{uint8_t(color.x), uint8_t(color.y), uint8_t(color.z)});
	}
	return palette;
};