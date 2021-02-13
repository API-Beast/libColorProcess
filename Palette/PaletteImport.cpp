#include "PaletteImport.h"
#include <fstream>
#include <iomanip>

namespace Palette
{
namespace GPL
{

	ColorPalette<sRGB_uint8> import_from_file(const char* filename)
	{
		std::ifstream file(filename, std::ios_base::in);
		return import_from_stream(file);	
	}

	ColorPalette<sRGB_uint8> import_from_stream(std::istream& input) 
	{
		ColorPalette<sRGB_uint8> palette;
		input.ignore(1024, '\n'); // # GIMP Palette or KOffice Palette
		while(input.good())
		{
			input >> std::skipws;
			if(input.peek() == '#') // Line comments
			{
				input.ignore(1024, '\n'); // Ignore the rest of the line
				continue;
			}
			if(input.peek() == '\n' || input.peek() == '\0' || input.peek() == std::istream::traits_type::eof()) // Empty lines
			{
				input.ignore(1);
				continue;
			}

			Vector3<int32_t> color;
			input >> color.x >> std::ws >> color.y >> std::ws >> color.z >> std::ws;
			input.ignore(1024, '\n'); // Ignore the rest of the line
			palette.push_back(sRGB_uint8{uint8_t(color.x), uint8_t(color.y), uint8_t(color.z)});
		}
		return palette;
	}

	void export_to_stream(std::ostream& output, const ColorPalette<sRGB_uint8>& data) 
	{
		output << "GIMP Palette\n";
		output << "# Generated using libColorProcess\n";
		int i = 0;
		for(auto& color : data)
		{
			output << std::right << std::setw(3) << int(color.red) << " "
			       << std::right << std::setw(3) << int(color.green) << " "
				   << std::right << std::setw(3) << int(color.blue) << " " << std::setw(0) << "Color_" << i << "\n";
			i += 1;
		}
	}

	void export_to_file(const char* filename, const ColorPalette<sRGB_uint8>& data)
	{
		std::ofstream file(filename, std::ios_base::out | std::ios_base::trunc);
		export_to_stream(file, data);
	}
}
}