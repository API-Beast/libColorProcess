#include <libColorTool.h>
#include <fstream>
#include "HtmlExporter.h"

ColorPalette<sRGB> import_gpl_file(const char* filename);

int main()
{
	printf("Importing palette.\n");
	ColorPalette<sRGB> pal = import_gpl_file("palette.gpl");
	printf("Palette has %lu colors.\n", pal.size());
	printf("Creating file 'export.html'.\n");
	HtmlExporter exporter("export.html");
	exporter.header("palette_operations Example");
	exporter.paragraph("Imported palette");
	exporter.palette(pal);
	printf("Sorting.\n");
	exporter.paragraph("Palette::sort Stats::perceptive_factors");
	exporter.palette(Palette::sort(pal, Stats::perceptive_factors));
	exporter.paragraph("Palette::sort Stats::srgb_factors");
	exporter.palette(Palette::sort(pal, Stats::srgb_factors));
	printf("Palette reduction.\n");
	exporter.paragraph("Palette::reduce_using_median_split(16) Palette::sort Stats::perceptive_factors");
	exporter.palette(Palette::sort(Palette::reduce_using_median_split(pal, 16, Stats::perceptive_factors), Stats::perceptive_factors));
	exporter.paragraph("Palette::reduce_using_median_split(16) Palette::sort Stats::srgb_factors");
	exporter.palette(Palette::sort(Palette::reduce_using_median_split(pal, 16, Stats::srgb_factors), Stats::srgb_factors));
	printf("Finished.\n");
	return 0;
}

ColorPalette<sRGB> import_gpl_file(const char* filename)
{
	std::fstream input(filename, std::ios_base::in);
	ColorPalette<sRGB> palette;
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