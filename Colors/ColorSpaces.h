#pragma once

#include "../Math/Vector3.h"
#include "../Math/CircularFloat.h"
#include <cstdint>
#include <cmath>
#include <utility>

namespace Colors
{

struct LinearRGBTemplate;
struct sRGBTemplate;
struct sRGBu8Template;
struct HSVTemplate;
struct LinearHSVTemplate;
struct HCYTemplate;

struct LinearRGBTemplate
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	operator sRGBTemplate() const;

	static constexpr float linear_component_to_sRGB_component(float val)
	{
		return val > 0.0031308 ? (1.055 * pow(val, 1.0/2.4)) - 0.055 : 12.92 * val;
	};
};

struct sRGBTemplate
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	operator LinearRGBTemplate() const;
	
	static constexpr float sRGB_component_to_linear_component(float val)
	{
		if(val > 0.04045)
			return pow((val + 0.055) / 1.055, 2.4);
		else
			return val / 12.92;
	};
};

struct sRGBu8Template
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;

	operator sRGBTemplate() const;
};

struct HSVTemplate
{
	CircularFloat hue = 0.0;
	float saturation = 0.0;
	float value = 0.0;

	constexpr HSVTemplate() = default;
	explicit HSVTemplate(const sRGBTemplate& rgbf);
};

struct LinearHSVTemplate
{
	CircularFloat hue = 0.0;
	float saturation = 0.0;
	float value = 0.0;

	constexpr LinearHSVTemplate() = default;
	explicit LinearHSVTemplate(const LinearRGBTemplate& rgbf);
};

struct HCYTemplate
{
	CircularFloat hue = 0.0;
	float chroma = 0.0;
	float luminance = 0.0;

	constexpr HCYTemplate() = default;
	explicit HCYTemplate(const LinearRGBTemplate& rgbf);

	//static float get_max_chroma(float luminance, CircularFloat hue);
	//static std::pair<float> get_luminance_limits(float chroma, CircularFloat hue);
};

using LinRGB = Vector3Mixin<LinearRGBTemplate, &LinearRGBTemplate::red, &LinearRGBTemplate::green, &LinearRGBTemplate::blue>;
using sRGB = Vector3Mixin<sRGBTemplate, &sRGBTemplate::red, &sRGBTemplate::green, &sRGBTemplate::blue>;
using sRGBu8 = Vector3Mixin<sRGBu8Template, &sRGBu8Template::red, &sRGBu8Template::green, &sRGBu8Template::blue>;
using HSV = Vector3Mixin<HSVTemplate, &HSVTemplate::hue, &HSVTemplate::saturation, &HSVTemplate::value>;
using LinearHSV = Vector3Mixin<LinearHSVTemplate, &LinearHSVTemplate::hue, &LinearHSVTemplate::saturation, &LinearHSVTemplate::value>;
using HCY = Vector3Mixin<HCYTemplate, &HCYTemplate::hue, &HCYTemplate::chroma, &HCYTemplate::luminance>;

}