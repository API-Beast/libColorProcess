#include "ColorSpaces.h"
#include "Colors.h"
#include <initializer_list>
#include <algorithm>
#include <iostream>

LinearRGB::operator sRGB() const
{
	return sRGB(red, green, blue).visit(&Colors::component_linear_to_sRGB);
};

sRGB::operator LinearRGB() const
{
	return LinearRGB(red, green, blue).visit(&Colors::component_sRGB_to_linear);
};

sRGB_uint8::operator sRGB() const
{
	return {red / float(UINT8_MAX), green / float(UINT8_MAX), blue / float(UINT8_MAX)};
};

uint32_t sRGB_uint8::to_int() 
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

		Conv(sRGB_uint8 p):integer(UINT32_MAX)
		{
			col.red = p.red;
			col.blue = p.blue;
			col.green = p.green;
		};
	};
	Conv c(*this);
	return c.integer;
};

sRGB::operator sRGB_uint8() const
{
	return {uint8_t(red * UINT8_MAX), uint8_t(green * UINT8_MAX), uint8_t(blue * UINT8_MAX)};
};

HSV::operator sRGB() const
{
	Vec3f saturated = Colors::saturate_hue(hue);
	Vec3f gray = Vec3f(1.0f) * (value - (value * saturation));
	Vec3f sum = saturated * value * saturation + gray;
	return sRGB{sum.x, sum.y, sum.z};
}

HSV::HSV(const sRGB& rgbf)
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

LinearHSV::operator LinearRGB() const
{
	HSV hsv{hue, saturation, value};
	sRGB rgb = hsv;
	return {rgb.red, rgb.green, rgb.blue};
}

LinearHSV::LinearHSV(const LinearRGB& rgbf)
{
	HSV hsv(sRGB{rgbf.red, rgbf.green, rgbf.blue});
	hue = hsv.hue;
	saturation = hsv.saturation;
	value = hsv.value;
}

HCY::HCY(const LinearRGB& rgbf)
{
	float minV = std::min({rgbf.red, rgbf.green, rgbf.blue});
	float maxV = std::max({rgbf.red, rgbf.green, rgbf.blue});
	chroma = maxV - minV;
	luminance = Colors::luminance709(rgbf);

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

HCY::operator LinearRGB() const
{
	Vec3f saturated = Colors::saturate_hue(hue) * chroma;
	float luminance_adjustment = luminance - Colors::luminance709(saturated);
	Vec3f gray = Vec3f(luminance_adjustment);
	Vec3f sum = saturated + gray;
	return LinearRGB{sum.x, sum.y, sum.z};
}

std::pair<float, float> HCY::get_luminance_limits(float chroma, float hue)
{
	float full_chroma_lum = Colors::luminance709(Colors::saturate_hue(hue));
	float lower_limit = full_chroma_lum * chroma;
	float upper_limit = (full_chroma_lum - 1.0) * chroma + 1.0;
	return std::make_pair(lower_limit, upper_limit);
}
