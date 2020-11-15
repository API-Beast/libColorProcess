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
	}

	void export_to_stream(const ColorPalette<sRGB_uint8>& data, std::ostream& output) 
	{
		int i = 0;
		output << "GIMP Palette\n";
		output << "# Generated using libColorTool\n";
		output.flush();
		for(auto& color : data)
		{
			output << std::setw(3) << color.red << " " << color.green << " " << color.blue << " " << std::setw(0) << "Color " << i << "\n";
			output.flush();
		}
	}

	void export_to_file(const char* filename, const ColorPalette<sRGB_uint8>& data)
	{
		std::ofstream file(filename, std::ios_base::out | std::ios_base::trunc);
		export_to_stream(data, file);
	}
}
}