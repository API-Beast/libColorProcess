#pragma once

#include "ColorSpaces.h"

namespace Colors
{

float luminance709(LinRGB color)
{
	return color.red * 0.2126f + color.green * 0.7152f + color.blue * 0.0722f;
};

float vibrance(LinRGB color)
{
	const float lum = luminance709(color);
	color = color.visit([lum](float val){ return (val<lum)?lum:val;});
	return (color - lum).length(); // Calculate distance
};

}