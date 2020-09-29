#pragma once

#include "ColorSpaces.h"
#include <initializer_list>
#include <algorithm> // Needed for initializer list version of std::max and std::min, maybe overkill?

namespace Colors
{

inline float luminance709(LinRGB color){ return color.red * 0.2126f + color.green * 0.7152f + color.blue * 0.0722f; };
inline float chroma(LinRGB rgbf){ return std::max({rgbf.red, rgbf.green, rgbf.blue}) - std::min({rgbf.red, rgbf.green, rgbf.blue}); };
inline float chroma(sRGB rgbf){ return std::max({rgbf.red, rgbf.green, rgbf.blue}) - std::min({rgbf.red, rgbf.green, rgbf.blue}); };
Vec3f saturate_hue(float hue);
float hue(Vec3f color);
float vibrance(LinRGB color);

}