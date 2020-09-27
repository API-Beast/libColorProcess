#pragma once

struct CircularFloat
{
	float value = 0.0;

	constexpr CircularFloat() = default;
	constexpr CircularFloat(float value):value(value){};
	constexpr operator float() const{ return value - (long long)(value); };
	constexpr CircularFloat operator+(float param) const { return CircularFloat(value + param); };
	constexpr CircularFloat operator-(float param) const { return CircularFloat(value - param); };
	constexpr CircularFloat operator*(float param) const { return CircularFloat(value * param); };
	constexpr CircularFloat operator/(float param) const { return CircularFloat(value / param); };
	CircularFloat& operator+=(float param){ value += param; return *this; };
	CircularFloat& operator-=(float param){ value -= param; return *this; };
	CircularFloat& operator*=(float param){ value *= param; return *this; };
	CircularFloat& operator/=(float param){ value /= param; return *this; };

	constexpr bool operator==(float other) const { return value == other; };
	constexpr bool operator!=(float other) const { return value != other; };
	constexpr bool operator<(float other) const  { return float(CircularFloat(value - other)) < 0.0f; };
	constexpr bool operator>(float other) const  { return float(CircularFloat(value - other)) > 0.0f; };
};