#pragma once

#include "ColorSpaces.h"

struct GenericColor
{
	enum class ColorSpace
	{
		LinRGB,
		sRGB,
		sRGBu8,
		sHSV,
		LinHSV,
		HCY
	};

	union
	{
		LinRGB linRgb;
		sRGB sRgb;
		sRGBu8 sRgbu8;
		sHSV sHsv;
		LinHSV linHsv;
		HCY hcy;
	};

	GenericColor()
	{
		linRgb = {1.0, 1.0, 1.0};
	};

	ColorSpace space = ColorSpace::LinRGB;

	template<typename T>
	operator T()
	{
		switch(space)
		{
			case ColorSpace::LinRGB:
				return colorspace_cast<T>(linRgb);
			case ColorSpace::sRgb:
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
	};

}