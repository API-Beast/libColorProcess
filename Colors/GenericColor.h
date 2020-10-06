#pragma once

#include "ColorSpaces.h"

class GenericColor
{
protected:
	union
	{
		LinRGB linRgb;
		sRGB sRgb;
		sRGBu8 sRgbu8;
		sHSV sHsv;
		LinHSV linHsv;
		HCY hcy;
		Vec3f genericVec3f;
		Vec3u8 genericVec3u8;
	};
	ColorSpace color_space = ColorSpace::LinRGB;

public:
	constexpr GenericColor():linRgb(1.0, 1.0, 1.0), color_space(ColorSpace::LinRGB){};
	constexpr GenericColor(LinRGB rgb):linRgb(rgb), color_space(ColorSpace::LinRGB){};
	constexpr GenericColor(sRGB rgb):sRgb(rgb), color_space(ColorSpace::sRGB){};
	constexpr GenericColor(sRGBu8 rgb):sRgbu8(rgb), color_space(ColorSpace::sRGBu8){};
	constexpr GenericColor(sHSV hsv):sHsv(hsv), color_space(ColorSpace::sHSV){};
	constexpr GenericColor(LinHSV hsv):linHsv(hsv), color_space(ColorSpace::LinHSV){};
	constexpr GenericColor(HCY hcy):linHsv(hcy), color_space(ColorSpace::HCY){};

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
		case ColorSpace::LinRGB:
			return colorspace_cast<T>(linRgb);
		case ColorSpace::sRGB:
			return colorspace_cast<T>(sRgb);
		case ColorSpace::sRGBu8:
			return colorspace_cast<T>(sRgbu8);
		case ColorSpace::sHSV:
			return colorspace_cast<T>(sHsv);
		case ColorSpace::LinHSV:
			return colorspace_cast<T>(linHsv);
		case ColorSpace::HCY:
			return colorspace_cast<T>(hcy);
	}
	return T(-1, -1, -1);
};