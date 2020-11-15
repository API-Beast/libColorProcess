#pragma once

#include "ColorSpaces.h"

namespace Colors
{
	// Red - (Blue + Green) / 2.0
	constexpr float redness(LinearRGB color){ return color.red - ((color.green + color.blue)/2.0); };
	// Green - (Blue + Red) / 2.0
	constexpr float greenness(LinearRGB color){ return color.green - ((color.red + color.blue)/2.0); };
	// Blue - (Green + Red) / 2.0
	constexpr float blueness(LinearRGB color){ return color.blue - ((color.green + color.red)/2.0); };
	// Luminance after Rec. 709 standard
	constexpr float luminance709(LinearRGB color){ return color.red * 0.2126f + color.green * 0.7152f + color.blue * 0.0722f; };
	// Value of saturated component of color
	constexpr float chroma(LinearRGB rgbf){ return rgbf.max_value() - rgbf.min_value(); };
	constexpr float chroma(sRGB rgbf){ return rgbf.max_value() - rgbf.min_value(); };

	// Creates a fully saturated generic vector from a hue.
	Vec3f saturate_hue(float hue);
	// Gets the hue of a generic vector.
	float hue(Vec3f color);
	// Distance between the color and a gray of equal luminance, subjectively perceived as vibrance.
	float vibrance(LinearRGB color);

	template<typename Color>
	constexpr Color validate_generic(Color c)
	{
		auto clamp = [](float v, float lo, float hi){ return (v < lo) ? lo : (hi < v) ? hi : v; };
		if constexpr(Color::num_components == 4)
			return {clamp(c.get_x(), 0.0f, 1.0f), clamp(c.get_y(), 0.0f, 1.0f), clamp(c.get_z(), 0.0f, 1.0f), clamp(c.get_w(), 0.0f, 1.0f)};
		else
			return {clamp(c.get_x(), 0.0f, 1.0f), clamp(c.get_y(), 0.0f, 1.0f), clamp(c.get_z(), 0.0f, 1.0f)};
	};

	constexpr LinearRGB validate(LinearRGB c){ return validate_generic(c); };
	constexpr sRGB validate(sRGB c){ return validate_generic(c); };
	constexpr LinearRGB_Alpha validate(LinearRGB_Alpha c){ return validate_generic(c); };
	constexpr sRGB_Alpha validate(sRGB_Alpha c){ return validate_generic(c); };

	[[deprecated("Validation does nothing for uint8 color-spaces. They are vulnerable to overflows but those are impossible to detect after the fact. If validation is important, convert your data to one of the floating point types instead.")]]
	constexpr sRGB_uint8 validate(sRGB_uint8 c){ return c; };
	[[deprecated("Validation does nothing for uint8 color-spaces. They are vulnerable to overflows but those are impossible to detect after the fact. If validation is important, convert your data to one of the floating point types instead.")]]
	constexpr sRGB_uint8_Alpha validate(sRGB_uint8_Alpha c){ return c; };

	constexpr LinearHSV validate(LinearHSV c)
	{
		c.hue = std::fmod(c.hue, 1.0);
		if(c.saturation < 0){ c.saturation *= -1; c.hue = 1.0f - c.hue; }
		c.value = std::min(std::max(c.value, 0.0f), 1.0f);
		return c;
	};

	constexpr HSV validate(HSV c)
	{
		c.hue = std::fmod(c.hue, 1.0);
		if(c.saturation < 0){ c.saturation *= -1; c.hue = 1.0f - c.hue; }
		c.value = std::min(std::max(c.value, 0.0f), 1.0f);
		return c;
	};

	[[deprecated("HCY Validation is not implemented yet! TODO FIXME")]]
	constexpr HCY validate(HCY c)
	{
		// TODO
		return c;
	};
}

template<typename To, typename From>
To colorspace_cast(const From& val)
{
	if constexpr(std::is_same<To, From>::value)
		return val;
	// Direct conversion if possible
	else if constexpr(std::is_convertible<From, To>::value)
		return static_cast<To>(From(val));
	// If the target is uint8 sRGB validate the color beforehand to avoid overflows.
	else if constexpr(std::is_same<To, sRGB_uint8>::value)
		return colorspace_cast<sRGB>(Colors::validate(val));
	else if constexpr(std::is_same<To, LinearRGB>::value || std::is_same<To, HSV>::value)
		return colorspace_cast<sRGB>(val);
	else if constexpr(std::is_same<To, LinearHSV>::value || std::is_same<To, HCY>::value)
		return colorspace_cast<LinearRGB>(val);
	else if constexpr(std::is_same<To, sRGB>::value)
	{
		if constexpr(std::is_convertible<From, LinearRGB>::value)
			return colorspace_cast<LinearRGB>(val);
		else if constexpr(std::is_convertible<From, sRGB_uint8>::value)
			return colorspace_cast<sRGB_uint8>(val);
		else
			return static_cast<To>(From(val));
	}
	else if constexpr(To::num_components == 3 && From::num_components == 4)
		return colorspace_cast<To>(val.strip_alpha());
	else if constexpr(To::num_components == 4 && From::num_components == 3)
		return colorspace_cast<decltype(To().strip_alpha())>(val);
	else
		return static_cast<To>(From(val));
};