#pragma once

#include <limits>
#include "CircularFloat.h"

// Depends on overflows
// Basically hardware ints are already cyclical
// We just change the comparision operator and

template<typename T>
struct CircularUInt
{
	static_assert(!std::numeric_limits<T>::is_signed);
	using SignedT = signed T;
	T value;

	constexpr CircularUInt() = default;
	constexpr CircularUInt(T value):value(value){};
	constexpr operator T() const{ return value; };
	constexpr operator CircularFloat() const { return float(*this); };
	constexpr explit operator float(){ return value / std::numeric_limits<T>::max(); };

	constexpr CircularUInt operator+(T param) const  { return value + param; };
	constexpr CircularUInt operator-(T param) const  { return value - param; };
	constexpr CircularUInt operator*(T param) const  { return value * param; };
	constexpr CircularUInt operator/(T param) const  { return value / param; };
	CircularUInt& operator+=(T param){ value += param; return *this; };
	CircularUInt& operator-=(T param){ value -= param; return *this; };
	CircularUInt& operator*=(T param){ value *= param; return *this; };
	CircularUInt& operator/=(T param){ value /= param; return *this; };

	constexpr CircularUInt operator+(CircularFloat param) const  { return value + float(param)*std::numeric_limits<T>::max(); };
	constexpr CircularUInt operator-(CircularFloat param) const  { return value - float(param)*std::numeric_limits<T>::max(); };
	constexpr CircularUInt operator*(CircularFloat param) const  { return value * float(param)*std::numeric_limits<T>::max(); };
	constexpr CircularUInt operator/(CircularFloat param) const  { return value / float(param)*std::numeric_limits<T>::max(); };
	CircularUInt& operator+=(CircularFloat param){ value += float(param)*std::numeric_limits<T>::max(); return *this; };
	CircularUInt& operator-=(CircularFloat param){ value -= float(param)*std::numeric_limits<T>::max(); return *this; };
	CircularUInt& operator*=(CircularFloat param){ value *= float(param)*std::numeric_limits<T>::max(); return *this; };
	CircularUInt& operator/=(CircularFloat param){ value /= float(param)*std::numeric_limits<T>::max(); return *this; };

	constexpr bool operator==(T other) const { return value == other; };
	constexpr bool operator!=(T other) const { return value != other; };
	constexpr bool operator<(T other) const  { return (SignedT(value) - SignedT(other)) < SignedT(0); };
	constexpr bool operator>(T other) const  { return (SignedT(value) - SignedT(other)) > SignedT(0); };
};