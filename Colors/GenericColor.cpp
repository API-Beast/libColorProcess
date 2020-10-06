#include "GenericColor.h"

GenericColor::GenericColor(Vec3f generic, ColorSpace space) 
{
	color_space = space;
	switch(space)
	{
		case ColorSpace::LinRGB:
			linRgb = LinRGB(generic.x, generic.y, generic.z);
		case ColorSpace::sRGB:
			sRgb = sRGB(generic.x, generic.y, generic.z);
		case ColorSpace::sRGBu8:
			sRgbu8 = sRGBu8(generic.x, generic.y, generic.z);
		case ColorSpace::sHSV:
			sHsv = sHSV(generic.x, generic.y, generic.z);
		case ColorSpace::LinHSV:
			linHsv = LinHSV(generic.x, generic.y, generic.z);
		case ColorSpace::HCY:
			hcy = HCY(generic.x, generic.y, generic.z);
	}
}

Vec3f GenericColor::get_vector() 
{
	if(color_space == ColorSpace::sRGBu8)
		return Vec3f(genericVec3u8.x, genericVec3u8.y, genericVec3u8.z);
	else
		return genericVec3f;
}

void GenericColor::convert_to(ColorSpace space)
{
	switch(space)
	{
		case ColorSpace::LinRGB:
			this->linRgb = *this;
			break;
		case ColorSpace::sRGB:
			this->sRgb = *this;
			break;
		case ColorSpace::sRGBu8:
			this->sRgbu8 = *this;
			break;
		case ColorSpace::sHSV:
			this->sHsv = *this;
			break;
		case ColorSpace::LinHSV:
			this->linHsv = *this;
			break;
		case ColorSpace::HCY:
			this->hcy = *this;
			break;
	}
	color_space = space;
}