#include "ColorSpaces.h"

namespace Colors
{

namespace
{

template<class T>
constexpr const T& clamp( T v, T lo, T hi )
{
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

}

LinearRGBTemplate::operator sRGBTemplate() const
{
	return {linear_component_to_sRGB_component(red), linear_component_to_sRGB_component(green), linear_component_to_sRGB_component(blue)};
};

sRGBTemplate::operator LinearRGBTemplate() const
{
	return {sRGB_component_to_linear_component(red), sRGB_component_to_linear_component(green), sRGB_component_to_linear_component(blue)};
};

sRGBu8Template::operator sRGBTemplate() const
{
	return {red / float(UINT8_MAX), green / float(UINT8_MAX), blue / float(UINT8_MAX)};
};

/*
LinearRGBTemplate::LinearRGBTemplate(const LinearHSVTemplate& hsv)
{

}

LinearRGBTemplate::LinearRGBTemplate(const HCYTemplate& hcy)
{

}

sRGBTemplate::sRGBTemplate(const HSVTemplate& hsv)
{
	Vec3f saturated = Vec3f(
		clamp<float>(std::abs(float(hsv.hue)*6.0f-3.0f) - 1.0f, 0.0f, 1.0f),
		clamp<float>(std::abs(float(hsv.hue + (1.0f / 3.0f * 2.0f))*6.0f-3.0f) - 1.0f, 0.0f, 1.0f),
		clamp<float>(std::abs(float(hsv.hue + (1.0f / 3.0f * 1.0f))*6.0f-3.0f) - 1.0f, 0.0f, 1.0f)
		) * hsv.value * hsv.saturation;
	Vec3f gray = Vec3f(1.0f, 1.0f, 1.0f) * (hsv.value - (hsv.value * hsv.saturation));
	Vec3f sum = saturated + gray;
	red = sum.x;
	green = sum.y;
	blue = sum.z;
}

HSVTemplate::HSVTemplate(const sRGBTemplate& rgbf)
{

}

LinearHSVTemplate::LinearHSVTemplate(const LinearRGBTemplate& rgbf)
{

}*/

}
