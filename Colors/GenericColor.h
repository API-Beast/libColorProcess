#pragma once

#include "ColorSpaces.h"

class GenericColor
{
protected:
	union
	{
		LinearRGB linRgb;
		sRGB sRgb;
		sRGB_uint8 sRgbu8;
		HSV Hsv;
		LinearHSV linHsv;
		HCY hcy;
		Vec3f genericVec3f;
		Vec3u8 genericVec3u8;
	};
	ColorSpace color_space = ColorSpace::LinearRGB;

public:
	constexpr GenericColor():linRgb(1.0, 1.0, 1.0), color_space(ColorSpace::LinearRGB){};
	constexpr GenericColor(LinearRGB rgb):linRgb(rgb), color_space(ColorSpace::LinearRGB){};
	constexpr GenericColor(sRGB rgb):sRgb(rgb), color_space(ColorSpace::sRGB){};
	constexpr GenericColor(sRGB_uint8 rgb):sRgbu8(rgb), color_space(ColorSpace::sRGB_uint8){};
	constexpr GenericColor(HSV hsv):Hsv(hsv), color_space(ColorSpace::HSV){};
	constexpr GenericColor(LinearHSV hsv):linHsv(hsv), color_space(ColorSpace::LinearHSV){};
	constexpr GenericColor(HCY hcy):hcy(hcy), color_space(ColorSpace::HCY){};

	GenericColor(Vec3f generic, ColorSpace p_space);

	ColorSpace get_color_space();
	void convert_to(ColorSpace space);
	Vec3f get_vector();

	template<typename T>
	operator T();
};

template<typename T>
GenericColor::operator T()
{
	switch(color_space)
	{
		case ColorSpace::LinearRGB:
			return colorspace_cast<T>(linRgb);
		case ColorSpace::sRGB:
			return colorspace_cast<T>(sRgb);
		case ColorSpace::sRGB_uint8:
			return colorspace_cast<T>(sRgbu8);
		case ColorSpace::HSV:
			return colorspace_cast<T>(Hsv);
		case ColorSpace::LinearHSV:
			return colorspace_cast<T>(linHsv);
		case ColorSpace::HCY:
			return colorspace_cast<T>(hcy);
	}
	return T(-1, -1, -1);
};