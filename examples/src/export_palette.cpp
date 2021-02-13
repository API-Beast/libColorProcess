#include <libColorProcess.h>

int main()
{
	ColorPalette<sRGB_uint8> input = Palette::GPL::import_from_file("source_palette.gpl");
	ImageData<sRGB_uint8_Alpha> output;

	Vec2i array_size = {16, int(std::ceil(input.size() / 16.0))};
	Vec2i block_size = 12;

	output.allocate(array_size*block_size, {0, 0, 0, 0});
	auto chunks = Iterate::chunks(output, block_size);

	int i = 0;
	for(int y = 0; y < array_size.y; y++)
	for(int x = 0; x < array_size.x; x++)
	for(auto& color : chunks[i++])
	if(i < input.size())
		color = input[i];

	Image::TGA::export_to_file("export_palette.tga", output);
}