#include "HtmlExporter.h"

HtmlExporter::HtmlExporter(const char* filename) 
{
	file = fopen(filename, "w");
	fprintf(file, "<html>\n");
	fprintf(file, "<link rel='stylesheet' href='style.css'>\n");
}

HtmlExporter::~HtmlExporter() 
{
	fprintf(file, "</html>\n");
	fclose(file);
}

void HtmlExporter::header(const char* text) 
{
	fprintf(file, "<h3>%s</h3>\n", text);
}

void HtmlExporter::paragraph(const char* text) 
{
	fprintf(file, "<p>%s</p>\n", text);
}