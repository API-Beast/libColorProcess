#pragma once

#include "ColorSpaces.h"

namespace Colors
{

	constexpr float redness(LinearRGB color){ return color.red - ((color.green + color.blue)/2.0); };
	constexpr float greenness(LinearRGB color){ return color.green - ((color.red + color.blue)/2.0); };
	constexpr float blueness(LinearRGB color){ return color.blue - ((color.green + color.red)/2.0); };
	constexpr float luminance709(LinearRGB color){ return color.red * 0.2126f + color.green * 0.7152f + color.blue * 0.0722f; };
	constexpr float chroma(LinearRGB rgbf){ return rgbf.max_value() - rgbf.min_value(); };
	constexpr float chroma(sRGB rgbf){ return rgbf.max_value() - rgbf.min_value(); };
	constexpr float component_sRGB_to_linear(float value){ return value > 0.0031308 ? (1.055 * std::pow(value, 1.0/2.4)) - 0.055 : 12.92 * value; };
	constexpr float component_linear_to_sRGB(float value){ return value > 0.04045 ? std::pow((value + 0.055) / 1.055, 2.4) : value / 12.92; };

	Vec3f saturate_hue(float hue);
	float hue(Vec3f color);
	float vibrance(LinearRGB color);
}