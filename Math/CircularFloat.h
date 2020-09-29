#pragma once

struct CircularFloat
{
	float value = 0.0;

	constexpr CircularFloat() = default;
	constexpr CircularFloat(float value):value(value){};
	constexpr CircularFloat(double value):value(value){};
	constexpr operator float() const{ float temp = 1.0 + (value - (long long)(value)); return temp - (long long)(temp); };

	constexpr CircularFloat operator+(float param) const { return CircularFloat(value + param); };
	constexpr CircularFloat operator-(float param) const { return CircularFloat(value - param); };
	constexpr CircularFloat operator*(float param) const { return CircularFloat(value * param); };
	constexpr CircularFloat operator/(float param) const { return CircularFloat(value / param); };
	constexpr CircularFloat operator+(double param) const { return CircularFloat(value + param); };
	constexpr CircularFloat operator-(double param) const { return CircularFloat(value - param); };
	constexpr CircularFloat operator*(double param) const { return CircularFloat(value * param); };
	constexpr CircularFloat operator/(double param) const { return CircularFloat(value / param); };

	CircularFloat& operator+=(float param){ value += param; return *this; };
	CircularFloat& operator-=(float param){ value -= param; return *this; };
	CircularFloat& operator*=(float param){ value *= param; return *this; };
	CircularFloat& operator/=(float param){ value /= param; return *this; };

	constexpr bool operator==(CircularFloat other) const { return operator float() == float(other); };
	constexpr bool operator!=(CircularFloat other) const { return operator float() != float(other); };
	constexpr bool operator==(float other) const { return operator float() == other; };
	constexpr bool operator!=(float other) const { return operator float() != other; };
	constexpr bool operator<(CircularFloat other) const  { return float(*this) < float(other); };
	constexpr bool operator>(CircularFloat other) const  { return float(*this) > float(other); };
	constexpr bool operator>=(CircularFloat other) const  { return float(*this) >= float(other); };
	constexpr bool operator<=(CircularFloat other) const  { return float(*this) <= float(other); };
	constexpr bool clockwise_of(CircularFloat other) const { return float(CircularFloat(value - other)) < 0.5f; };
	constexpr bool counter_clockwise_of(CircularFloat other) const { return float(CircularFloat(value - other)) > 0.5f; };
};