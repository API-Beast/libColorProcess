#pragma once

#include "../Math/Vector3.h"
#include <cstdint>
#include <cmath>
#include <utility>

namespace Colors
{

struct LinearRGBBase;
struct sRGBBase;
struct sRGBu8Base;
struct sHSVBase;
struct LinearHSVBase;
struct HCYBase;

struct LinearRGBBase
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	constexpr LinearRGBBase() = default;
	constexpr LinearRGBBase(float r, float g, float b):red(r),green(g),blue(b){};
	constexpr LinearRGBBase(const Vector3Base<float>& vec):red(vec.x),green(vec.y),blue(vec.z){};
	operator sRGBBase() const;

	static constexpr float linear_component_to_sRGB_component(float val)
	{
		return val > 0.0031308 ? (1.055 * pow(val, 1.0/2.4)) - 0.055 : 12.92 * val;
	};
};

struct sRGBBase
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	operator LinearRGBBase() const;
	operator sRGBu8Base() const;
	
	static constexpr float sRGB_component_to_linear_component(float val)
	{
		if(val > 0.04045)
			return pow((val + 0.055) / 1.055, 2.4);
		else
			return val / 12.92;
	};
};

struct sRGBu8Base
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;

	operator sRGBBase() const;
};

struct HSVBase
{
	float hue = 0.0;
	float saturation = 0.0;
	float value = 0.0;

	constexpr HSVBase() = default;
	constexpr HSVBase(float h, float s, float v):hue(h),saturation(s),value(v){};
	HSVBase(const sRGBBase& rgbf);
	operator sRGBBase() const;
};

struct LinearHSVBase
{
	float hue = 0.0;
	float saturation = 0.0;
	float value = 0.0;

	constexpr LinearHSVBase() = default;
	constexpr LinearHSVBase(float h, float s, float v):hue(h),saturation(s),value(v){};
	explicit LinearHSVBase(const LinearRGBBase& rgbf);
	operator LinearRGBBase() const;
};

struct HCYBase
{
	float hue = 0.0;
	float chroma = 0.0;
	float luminance = 0.0;

	constexpr HCYBase() = default;
	explicit HCYBase(const LinearRGBBase& rgbf);
	operator LinearRGBBase() const;

	static std::pair<float, float> get_luminance_limits(float chroma, float hue);
};

using LinRGB = Vector3Mixin<LinearRGBBase, &LinearRGBBase::red, &LinearRGBBase::green,      &LinearRGBBase::blue >;
using LinHSV = Vector3Mixin<LinearHSVBase, &LinearHSVBase::hue, &LinearHSVBase::saturation, &LinearHSVBase::value>;
using HCY    = Vector3Mixin<HCYBase,       &HCYBase::hue,       &HCYBase::chroma,           &HCYBase::luminance  >;

using sRGB   = Vector3Mixin<sRGBBase,   &sRGBBase::red,   &sRGBBase::green,      &sRGBBase::blue  >;
using sRGBu8 = Vector3Mixin<sRGBu8Base, &sRGBu8Base::red, &sRGBu8Base::green,    &sRGBu8Base::blue>;
using sHSV   = Vector3Mixin<HSVBase,    &HSVBase::hue,    &HSVBase::saturation,  &HSVBase::value >;

}