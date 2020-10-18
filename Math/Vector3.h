#pragma once
#include "Vector3Macro.h"

template<typename T>
struct Vector3
{
	T x;
	T y;
	T z;

	VECTOR3_CONSTRUCTORS(Vector3, x, y, z);
	VECTOR3_MEMBER_FUNCTIONS(Vector3<T>, x, y, z);
};

VECTOR3_OPERATORS_T(Vector3<T>, x, y, z, template<typename T>);
VECTOR3_FUNCTIONS_T(Vector3<T>, x, y, z, template<typename T>);

using Vec3f  = Vector3<float>;
using Vec3u8 = Vector3<unsigned char>;