#pragma once
#include <cstdio>
#include <Colors/ColorSpaces.h>
#include <Palette/Palette.h>

struct HtmlExporter
{
	FILE* file;
	HtmlExporter(const char* filename);
	~HtmlExporter();

	void header(const char* h);
	void paragraph(const char* text);
	template<typename T>
	void palette(const ColorPalette<T>& pal)
	{
		fflush(file);
		fprintf(file, "<div class=\"palette\">\n");
		for(T entry : pal)
		{
			sRGBu8 rgb = colorspace_cast<sRGBu8>(entry);
			fprintf(file, "<span class=\"entry\" style=\"background-color:rgb(%u, %u, %u);\"></span>", rgb.red, rgb.green, rgb.blue);
			fflush(file);
		}
		fprintf(file, "</div>\n");
		fflush(file);
	}
	
};