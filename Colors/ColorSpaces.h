#pragma once

#include "../Math/Vector3.h"
#include "../Math/Vector4Macro.h"
#include <cstdint>
#include <cmath>
#include <utility>
#include <iostream>

struct LinearRGB;
struct sRGB;
struct sRGB_uint8;

struct LinearRGB_Alpha;
struct sRGB_Alpha;
struct sRGB_uint8_Alpha;

struct HSV;
struct LinearHSV;
struct HCY;

struct alignas(16) LinearRGB
{
	float red = 1.0;
	float green = 1.0;
	float blue = 1.0;

	constexpr LinearRGB(Vec3f vec):red(vec.x),green(vec.y),blue(vec.z){};
	constexpr operator sRGB();

	VECTOR3_CONSTRUCTORS(LinearRGB, red, green, blue);
	VECTOR3_MEMBER_FUNCTIONS(LinearRGB, red, green, blue);
};
VECTOR3_OPERATORS(LinearRGB, red, green, blue);
VECTOR3_FUNCTIONS(LinearRGB, red, green, blue);

struct alignas(16) sRGB
{
	float red = 1.0;
	float green = 1.0;
	float blue = 1.0;

	constexpr operator LinearRGB();
	constexpr operator sRGB_uint8();

	VECTOR3_CONSTRUCTORS(sRGB, red, green, blue);
	VECTOR3_MEMBER_FUNCTIONS(sRGB, red, green, blue);
};
VECTOR3_OPERATORS(sRGB, red, green, blue);
VECTOR3_FUNCTIONS(sRGB, red, green, blue);

struct alignas(4) sRGB_uint8
{
	uint8_t red = 255;
	uint8_t green = 255;
	uint8_t blue = 255;

	constexpr operator sRGB(){ return {red / float(UINT8_MAX), green / float(UINT8_MAX), blue / float(UINT8_MAX)}; };
	uint32_t to_int();

	VECTOR3_CONSTRUCTORS(sRGB_uint8, red, green, blue);
	VECTOR3_MEMBER_FUNCTIONS(sRGB_uint8, red, green, blue);
};
VECTOR3_OPERATORS(sRGB_uint8, red, green, blue);
VECTOR3_FUNCTIONS(sRGB_uint8, red, green, blue);

struct alignas(16) HSV
{
	float hue = 0.0;
	float saturation = 0.0;
	float value = 1.0;

	HSV(sRGB rgbf);
	operator sRGB() const;

	VECTOR3_CONSTRUCTORS(HSV, hue, saturation, value);
	VECTOR3_MEMBER_FUNCTIONS(HSV, hue, saturation, value);
};
VECTOR3_OPERATORS(HSV, hue, saturation, value);
VECTOR3_FUNCTIONS(HSV, hue, saturation, value);

struct alignas(16) LinearHSV
{
	float hue = 0.0;
	float saturation = 0.0;
	float value = 1.0;

	LinearHSV(LinearRGB rgbf);
	operator LinearRGB() const;

	VECTOR3_CONSTRUCTORS(LinearHSV, hue, saturation, value);
	VECTOR3_MEMBER_FUNCTIONS(LinearHSV, hue, saturation, value);
};
VECTOR3_OPERATORS(LinearHSV, hue, saturation, value);
VECTOR3_FUNCTIONS(LinearHSV, hue, saturation, value);

struct alignas(16) HCY
{
	float hue = 0.0;
	float chroma = 0.0;
	float luminance = 1.0;

	HCY(LinearRGB rgbf);
	operator LinearRGB() const;

	static std::pair<float, float> get_luminance_limits(float chroma, float hue);

	VECTOR3_CONSTRUCTORS(HCY, hue, chroma, luminance);
	VECTOR3_MEMBER_FUNCTIONS(HCY, hue, chroma, luminance);
};
VECTOR3_OPERATORS(HCY, hue, chroma, luminance);
VECTOR3_FUNCTIONS(HCY, hue, chroma, luminance);

template<typename T, typename P>
T colorspace_cast(const P& val)
{
	if constexpr(std::is_same<T, P>::value)
		return val;
	// Direct conversion if possible
	else if constexpr(std::is_convertible<P, T>::value)
		return static_cast<T>(P(val));
	else if constexpr(std::is_same<T, LinearRGB>::value || std::is_same<T, sRGB_uint8>::value || std::is_same<T, HSV>::value)
		return colorspace_cast<sRGB>(val);
	else if constexpr(std::is_same<T, LinearHSV>::value || std::is_same<T, HCY>::value)
		return colorspace_cast<LinearRGB>(val);
	else if constexpr(std::is_same<T, sRGB>::value)
	{
		if constexpr(std::is_convertible<P, LinearRGB>::value)
			return colorspace_cast<LinearRGB>(val);
		else if constexpr(std::is_convertible<P, sRGB_uint8>::value)
			return colorspace_cast<sRGB_uint8>(val);
		else
			return static_cast<T>(P(val));
	}
	else
		return static_cast<T>(P(val));
};

struct LinearRGB_Alpha;
struct sRGB_Alpha;
struct sRGB_uint8_Alpha;

struct alignas(16) LinearRGB_Alpha
{
	float red = 1.0;
	float green = 1.0;
	float blue = 1.0;
	float alpha = 1.0;
	
	constexpr LinearRGB_Alpha(LinearRGB rgb, float a = 1.0):red(rgb.red), green(rgb.green), blue(rgb.blue), alpha(a){};
	constexpr operator LinearRGB(){ return {red, green, blue}; };
	constexpr operator sRGB_Alpha();

	VECTOR4_CONSTRUCTORS(LinearRGB_Alpha, red, green, blue, alpha);
	VECTOR4_MEMBER_FUNCTIONS(LinearRGB_Alpha, red, green, blue, alpha);
};
VECTOR4_OPERATORS(LinearRGB_Alpha, red, green, blue, alpha);
VECTOR4_FUNCTIONS(LinearRGB_Alpha, red, green, blue, alpha);

struct alignas(16) sRGB_Alpha
{
	float red = 1.0;
	float green = 1.0;
	float blue = 1.0;
	float alpha = 1.0;

	constexpr sRGB_Alpha(sRGB rgb, float a = 1.0):red(rgb.red), green(rgb.green), blue(rgb.blue), alpha(a){};
	constexpr operator LinearRGB_Alpha(){ return LinearRGB_Alpha(static_cast<sRGB>(*this), alpha); };
	constexpr operator sRGB_uint8_Alpha();
	constexpr operator sRGB(){ return {red, green, blue}; };

	VECTOR4_CONSTRUCTORS(sRGB_Alpha, red, green, blue, alpha);
	VECTOR4_MEMBER_FUNCTIONS(sRGB_Alpha, red, green, blue, alpha);
};
VECTOR4_OPERATORS(sRGB_Alpha, red, green, blue, alpha);
VECTOR4_FUNCTIONS(sRGB_Alpha, red, green, blue, alpha);

struct alignas(4) sRGB_uint8_Alpha
{
	uint8_t red = 255;
	uint8_t green = 255;
	uint8_t blue = 255;
	uint8_t alpha = 255;

	constexpr sRGB_uint8_Alpha(sRGB_uint8 rgb, uint8_t a = 255):red(rgb.red), green(rgb.green), blue(rgb.blue), alpha(a){};
	constexpr operator sRGB_Alpha(){ return sRGB_Alpha(red / float(UINT8_MAX), green / float(UINT8_MAX), blue / float(UINT8_MAX), alpha / float(UINT8_MAX)); };
	constexpr operator sRGB_uint8(){ return {red, green, blue}; };
	uint32_t to_int();

	VECTOR4_CONSTRUCTORS(sRGB_uint8_Alpha, red, green, blue, alpha);
	VECTOR4_MEMBER_FUNCTIONS(sRGB_uint8_Alpha, red, green, blue, alpha);
};
VECTOR4_OPERATORS(sRGB_uint8_Alpha, red, green, blue, alpha);
VECTOR4_FUNCTIONS(sRGB_uint8_Alpha, red, green, blue, alpha);

namespace Colors
{
	constexpr float component_sRGB_to_linear(float value){ return value > 0.0031308 ? (1.055 * std::pow(value, 1.0/2.4)) - 0.055 : 12.92 * value; };
	constexpr float component_linear_to_sRGB(float value){ return value > 0.04045 ? std::pow((value + 0.055) / 1.055, 2.4) : value / 12.92; };
};

constexpr sRGB::operator LinearRGB()
{
	return LinearRGB(red, green, blue).visit(&Colors::component_sRGB_to_linear);
};

constexpr sRGB::operator sRGB_uint8()
{
	return sRGB_uint8(red * UINT8_MAX, green * UINT8_MAX, blue * UINT8_MAX);
};

constexpr LinearRGB::operator sRGB()
{
	return sRGB(red, green, blue).visit(&Colors::component_linear_to_sRGB);
};

constexpr LinearRGB_Alpha::operator sRGB_Alpha()
{
	return sRGB_Alpha(static_cast<LinearRGB>(*this), alpha);
}

constexpr sRGB_Alpha::operator sRGB_uint8_Alpha()
{
	return sRGB_uint8_Alpha(red * UINT8_MAX, green * UINT8_MAX, blue  * UINT8_MAX, alpha * UINT8_MAX);
}

enum class ColorSpace
{
	LinearRGB,
	sRGB,
	sRGB_uint8,
	HSV,
	LinearHSV,
	HCY
};