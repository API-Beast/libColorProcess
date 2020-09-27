#pragma once
#include "Vector3Mixin.h"

template<typename T>
struct Vector3Template
{
	T x;
	T y;
	T z;
};

using Vec3f = Vector3Mixin<Vector3Template<float>, &Vector3Template<float>::x, &Vector3Template<float>::x, &Vector3Template<float>::x>;