#pragma once

#include "../Math/Vector3.h"
#include <cstdint>
#include <cmath>
#include <utility>
#include <iostream>

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
	uint32_t to_int();
};

struct HSVBase
{
	float hue = 0.0;
	float saturation = 0.0;
	float value = 0.0;

	constexpr HSVBase() = default;
	constexpr HSVBase(float h, float s, float v):hue(h),saturation(s),value(v){};
	explicit HSVBase(const sRGBBase& rgbf);
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

template<typename T, typename P>
T colorspace_cast(const P& val)
{
	// We expect all T's and P's to be Vector3Mixins
	using TBase = typename T::BaseType;
	using PBase = typename P::BaseType;
	if constexpr(std::is_same<T, P>::value)
		return val;
	// Direct conversion if possible
	else if constexpr(std::is_convertible<PBase, TBase>::value)
		return static_cast<TBase>(PBase(val));
	else if constexpr(std::is_same<T, LinRGB>::value || std::is_same<T, sRGBu8>::value || std::is_same<T, sHSV>::value)
		return colorspace_cast<sRGB>(val);
	else if constexpr(std::is_same<T, LinHSV>::value || std::is_same<T, HCY>::value)
		return colorspace_cast<LinRGB>(val);
	else if constexpr(std::is_same<T, sRGB>::value)
	{
		if constexpr(std::is_convertible<PBase, LinearRGBBase>::value)
			return colorspace_cast<LinRGB>(val);
		else if constexpr(std::is_convertible<PBase, sRGBu8Base>::value)
			return colorspace_cast<sRGBu8>(val);
		else
			return static_cast<TBase>(PBase(val));
	}
	else
		return static_cast<TBase>(PBase(val));
};

enum class ColorSpace
{
	LinRGB,
	sRGB,
	sRGBu8,
	sHSV,
	LinHSV,
	HCY
};