#include "GenericColor.h"

GenericColor::GenericColor(Vec3f generic, ColorSpace space) 
{
	color_space = space;
	switch(space)
	{
		case ColorSpace::LinearRGB:
			linRgb = LinearRGB(generic.x, generic.y, generic.z);
		case ColorSpace::sRGB:
			sRgb = sRGB(generic.x, generic.y, generic.z);
		case ColorSpace::sRGB_uint8:
			sRgbu8 = sRGB_uint8(generic.x, generic.y, generic.z);
		case ColorSpace::HSV:
			Hsv = HSV(generic.x, generic.y, generic.z);
		case ColorSpace::LinearHSV:
			linHsv = LinearHSV(generic.x, generic.y, generic.z);
		case ColorSpace::HCY:
			hcy = HCY(generic.x, generic.y, generic.z);
	}
}

Vec3f GenericColor::get_vector() 
{
	if(color_space == ColorSpace::sRGB_uint8)
		return Vec3f(genericVec3u8.x, genericVec3u8.y, genericVec3u8.z);
	else
		return genericVec3f;
}

void GenericColor::convert_to(ColorSpace space)
{
	switch(space)
	{
		case ColorSpace::LinearRGB:
			this->linRgb = *this;
			break;
		case ColorSpace::sRGB:
			this->sRgb = *this;
			break;
		case ColorSpace::sRGB_uint8:
			this->sRgbu8 = *this;
			break;
		case ColorSpace::HSV:
			this->Hsv = *this;
			break;
		case ColorSpace::LinearHSV:
			this->linHsv = *this;
			break;
		case ColorSpace::HCY:
			this->hcy = *this;
			break;
	}
	color_space = space;
}