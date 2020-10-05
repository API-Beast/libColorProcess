#include "ColorSpaces.h"
#include "Colors.h"
#include <initializer_list>
#include <algorithm>
#include <iostream>

namespace
{

template<class T>
constexpr const T& clamp( T v, T lo, T hi )
{
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

}

LinearRGBBase::operator sRGBBase() const
{
	return {linear_component_to_sRGB_component(red), linear_component_to_sRGB_component(green), linear_component_to_sRGB_component(blue)};
};

sRGBBase::operator LinearRGBBase() const
{
	return {sRGB_component_to_linear_component(red), sRGB_component_to_linear_component(green), sRGB_component_to_linear_component(blue)};
};

sRGBu8Base::operator sRGBBase() const
{
	return {red / float(UINT8_MAX), green / float(UINT8_MAX), blue / float(UINT8_MAX)};
};

uint32_t sRGBu8Base::to_int() 
{
	union Conv
	{
		struct
		{
			uint8_t alpha;
			uint8_t blue;
			uint8_t green;
			uint8_t red;
		} col;
		uint32_t integer;

		Conv(sRGBu8Base p):integer(UINT32_MAX)
		{
			col.red = p.red;
			col.blue = p.blue;
			col.green = p.green;
		};
	};
	Conv c(*this);
	return c.integer;
};

sRGBBase::operator sRGBu8Base() const
{
	return {uint8_t(red * UINT8_MAX), uint8_t(green * UINT8_MAX), uint8_t(blue * UINT8_MAX)};
};

HSVBase::operator sRGBBase() const
{
	Vec3f saturated = saturate_hue(hue);
	Vec3f gray = Vec3f(1.0f) * (value - (value * saturation));
	Vec3f sum = saturated * value * saturation + gray;
	return sRGBBase{sum.x, sum.y, sum.z};
}

HSVBase::HSVBase(const sRGBBase& rgbf)
{
	float minV = std::min(rgbf.red, std::min(rgbf.green, rgbf.blue));
	float maxV = std::max(rgbf.red, std::max(rgbf.green, rgbf.blue));
	float deltaV = maxV - minV;
	value = maxV;

	if(maxV == 0.0f || deltaV == 0.0f)
		return;

	saturation = deltaV / maxV;

	const double step = 1.0 / 6.0;
	if(rgbf.red == maxV)
		hue =             step * (rgbf.green - rgbf.blue) / deltaV;
	else if(rgbf.green == maxV)
		hue = 2.0 / 6.0 + step * (rgbf.blue - rgbf.red) / deltaV;
	else
		hue = 4.0 / 6.0 + step * (rgbf.red - rgbf.green) / deltaV;
}

LinearHSVBase::operator LinearRGBBase() const
{
	HSVBase hsv{hue, saturation, value};
	sRGBBase rgb = hsv;
	return {rgb.red, rgb.green, rgb.blue};
}

LinearHSVBase::LinearHSVBase(const LinearRGBBase& rgbf)
{
	HSVBase hsv(sRGBBase{rgbf.red, rgbf.green, rgbf.blue});
	hue = hsv.hue;
	saturation = hsv.saturation;
	value = hsv.value;
}

HCYBase::HCYBase(const LinearRGBBase& rgbf)
{
	float minV = std::min({rgbf.red, rgbf.green, rgbf.blue});
	float maxV = std::max({rgbf.red, rgbf.green, rgbf.blue});
	chroma = maxV - minV;
	luminance = luminance709(rgbf);

	if(maxV == 0.0f || chroma == 0.0f)
		return;

	const double step = 1.0 / 6.0;
	if(rgbf.red == maxV)
		hue =             step * (rgbf.green - rgbf.blue) / chroma;
	else if(rgbf.green == maxV)
		hue = 2.0 / 6.0 + step * (rgbf.blue - rgbf.red) / chroma;
	else
		hue = 4.0 / 6.0 + step * (rgbf.red - rgbf.green) / chroma;
}

HCYBase::operator LinearRGBBase() const
{
	Vec3f saturated = saturate_hue(hue) * chroma;
	float luminance_adjustment = luminance - luminance709(saturated);
	Vec3f gray = Vec3f(luminance_adjustment);
	Vec3f sum = saturated + gray;
	return LinearRGBBase{sum.x, sum.y, sum.z};
}

std::pair<float, float> HCYBase::get_luminance_limits(float chroma, float hue)
{
	float full_chroma_lum = luminance709(saturate_hue(hue));
	float lower_limit = full_chroma_lum * chroma;
	float upper_limit = (full_chroma_lum - 1.0) * chroma + 1.0;
	return std::make_pair(lower_limit, upper_limit);
}
