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

	Vec3f saturate_hue(float hue);
	float hue(Vec3f color);
	float vibrance(LinearRGB color);
}