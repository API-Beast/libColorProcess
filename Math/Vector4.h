#pragma once
#include "Vector4Macro.h"

template<typename T>
struct Vector4
{
	T x = T(0);
	T y = T(0);
	T z = T(0);
	T w = T(0);

	VECTOR4_CONSTRUCTORS(Vector4, x, y, z, w);
	VECTOR4_MEMBER_FUNCTIONS(Vector4<T>, x, y, z, w);
};

VECTOR4_OPERATORS_T(Vector4<T>, x, y, z, w, template<typename T>);
VECTOR4_FUNCTIONS_T(Vector4<T>, x, y, z, w, template<typename T>);

using Vec4f  = Vector4<float>;
using Vec4u8 = Vector4<unsigned char>;