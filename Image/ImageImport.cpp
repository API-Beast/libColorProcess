#include "ImageImport.h"

#include <cassert>
#include <cstdio>
#include <new>

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace Image
{	
	namespace TGA
	{
		#pragma scalar_storage_order little_endian
		#pragma pack(push, 1)
		struct Header
		{
			uint8_t id_length = 0;
			uint8_t color_map_type = 0;
			uint8_t image_type = 2; // Uncompressed RGB image
			// Color Map
			uint16_t first_index = 0;
			uint16_t number_of_entries = 0;
			uint8_t color_map_bits_per_pixel = 0;
			// Image Description
			uint16_t x_origin = 0;
			uint16_t y_origin = 0;
			uint16_t width  = 0;
			uint16_t height = 0;
			uint8_t image_bits_per_pixel = 32;
			uint8_t bits_per_component:3,
					reserved:1,
					screen_origin:1,
					interleaving_mode:2;
		};
		#pragma pack(pop)
		#pragma scalar_storage_order default
	}

	ImageData<sRGB_uint8_Alpha> TGA::import_from_file(const char* filename) 
	{
		std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
		return import_from_stream(file);	
	}
	
	ImageData<sRGB_uint8_Alpha> TGA::import_from_stream(std::istream& input) 
	{
		TGA::Header hdr;
		input.read((char*)&hdr, sizeof(hdr));
		if(hdr.color_map_type != 0)
		{
			// WIP
			std::puts("TGA: Color maps are currently not supported.\n");
			return {};
		}
		if(hdr.image_type != 2)
		{
			// WIP
			std::puts("TGA: Only uncompressed 32-bit RGB images are currently supported.\n");
			return {};
		}
		// ID block
		input.ignore(hdr.id_length);
		// Color map block
		//input.ignore(...);
		// Color data block
		ImageData<sRGB_uint8_Alpha> retVal(hdr.width, hdr.height);
		switch(hdr.image_bits_per_pixel)
		{
			case 32:
				for(int i = 0; i < hdr.width * hdr.height; i++)
				{
					uint8_t buffer[4];
					input.read((char*)buffer, 4);
					retVal[i] = sRGB_uint8_Alpha(buffer[2], buffer[1], buffer[0], buffer[3]);
				}
				break;
			case 24:
				for(int i = 0; i < hdr.width * hdr.height; i++)
				{
					uint8_t buffer[3];
					input.read((char*)buffer, 3);
					retVal[i] = sRGB_uint8_Alpha(buffer[2], buffer[1], buffer[0], 255);
				}
				break;
			case 16:
				for(int i = 0; i < hdr.width * hdr.height; i++)
				{
					struct { uint16_t alpha:1,red:5,green:5,blue:5;  } rgb16_buffer;
					input.read((char*)&rgb16_buffer, 2);
					retVal[i] = sRGB_uint8_Alpha(rgb16_buffer.red << 3, rgb16_buffer.green << 3, rgb16_buffer.blue << 3, rgb16_buffer.alpha << 8);
				}
				break;
			default:
				std::puts("TGA: The bitdepth is not supported.");
				break;
		}
		return retVal;
	}
	
	void TGA::export_to_file(const ImageData<sRGB_uint8_Alpha>& data, const char* filename) 
	{
		std::ofstream file(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		export_to_stream(data, file);
	}
	
	void TGA::export_to_stream(const ImageData<sRGB_uint8_Alpha>& data, std::ostream& output) 
	{
		TGA::Header hdr;
		hdr.width = data.size.x;
		hdr.height = data.size.y;
		hdr.bits_per_component = 0;
		hdr.reserved = 0;
		hdr.screen_origin = 0;
		hdr.interleaving_mode = 0;

		output.write((char*)&hdr, sizeof(hdr));
		// 0 size image identification
		// 0 size color map data
		for(const auto& color : data)
		{
			// Endian weirdness, TGA seems to have stored the color as integer, inheriting the endianness
			uint8_t buffer[4] = {color.blue, color.green, color.red, color.alpha};
			output.write((char*)buffer, 4);
		}
	}
}