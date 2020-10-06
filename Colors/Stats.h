#pragma once

#include "ColorSpaces.h"
#include "Colors.h"
#include <array>

namespace Stats
{

inline std::array<float, 5> perceptive_factors(LinRGB color)
{
	return {Colors::luminance709(color), Colors::vibrance(color), Colors::redness(color), Colors::greenness(color), Colors::blueness(color)};
};

constexpr std::array<float, 3> linrgb_factors(LinRGB color)
{
	return {color.red, color.green, color.blue};
};

constexpr std::array<float, 3> srgb_factors(sRGB color)
{
	return {color.red, color.green, color.blue};
};

}