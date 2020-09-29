#pragma once
#include "Vector3Mixin.h"

template<typename T>
struct Vector3Base
{
	T x;
	T y;
	T z;

	constexpr Vector3Base() = default;
	constexpr Vector3Base(T x, T y, T z):x(x),y(y),z(z){};
};

using Vec3f = Vector3Mixin<Vector3Base<float>, &Vector3Base<float>::x, &Vector3Base<float>::y, &Vector3Base<float>::z>;