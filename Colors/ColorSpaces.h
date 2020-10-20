#pragma once

#include "../Math/Vector3.h"
#include <cstdint>
#include <cmath>
#include <utility>
#include <iostream>

struct LinearRGB;
struct sRGB;
struct sRGB_uint8;
struct HSV;
struct LinearHSV;
struct HCY;

struct alignas(16) LinearRGB
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	constexpr LinearRGB(const Vec3f& vec):red(vec.x),green(vec.y),blue(vec.z){};
	
	operator sRGB() const;

	VECTOR3_CONSTRUCTORS(LinearRGB, red, green, blue);
	VECTOR3_MEMBER_FUNCTIONS(LinearRGB, red, green, blue);
};
VECTOR3_OPERATORS(LinearRGB, red, green, blue);
VECTOR3_FUNCTIONS(LinearRGB, red, green, blue);

struct alignas(16) sRGB
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	operator LinearRGB() const;
	operator sRGB_uint8() const;

	VECTOR3_CONSTRUCTORS(sRGB, red, green, blue);
	VECTOR3_MEMBER_FUNCTIONS(sRGB, red, green, blue);
};
VECTOR3_OPERATORS(sRGB, red, green, blue);
VECTOR3_FUNCTIONS(sRGB, red, green, blue);

struct alignas(4) sRGB_uint8
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;

	operator sRGB() const;
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
	float value = 0.0;

	HSV(const sRGB& rgbf);
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
	float value = 0.0;

	LinearHSV(const LinearRGB& rgbf);
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
	float luminance = 0.0;

	HCY(const LinearRGB& rgbf);
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

enum class ColorSpace
{
	LinearRGB,
	sRGB,
	sRGB_uint8,
	HSV,
	LinearHSV,
	HCY
};