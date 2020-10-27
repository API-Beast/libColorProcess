#pragma once
#include "../Math/Vector4Macro.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include <algorithm>
#include "yestest.h"

template<typename T>
typename std::enable_if<T::is_vector3_type, void>::type print_value(const T& f) { printf("{"); YesTest::print_value(f.get_x(), f.get_y(), f.get_z()); printf("}"); }

template<typename T>
inline typename std::enable_if<T::is_vector3_type, int>::type ulp_difference(T a, T b)
{
	return std::max({YesTest::ulp_difference(a.get_x(), b.get_x()), YesTest::ulp_difference(a.get_y(), b.get_y()), YesTest::ulp_difference(a.get_z(), b.get_z())});
}

template<typename T>
typename std::enable_if<T::is_vector4_type, void>::type print_value(const T& f) { printf("{"); YesTest::print_value(f.get_x(), f.get_y(), f.get_z(), f.get_a()); printf("}"); }

template<typename T>
inline typename std::enable_if<T::is_vector4_type, int>::type ulp_difference(T a, T b)
{
	return std::max({YesTest::ulp_difference(a.get_x(), b.get_x()), YesTest::ulp_difference(a.get_y(), b.get_y()), YesTest::ulp_difference(a.get_z(), b.get_z()), YesTest::ulp_difference(a.get_a(), b.get_a())});
}

template<typename T>
void print_value(Vector2<T> f) { printf("{"); YesTest::print_value(f.x, f.y); printf("}"); }

template<typename T>
inline int ulp_difference(Vector2<T> a, Vector2<T> b)
{
	return std::max({YesTest::ulp_difference(a.x, b.x), YesTest::ulp_difference(a.y, b.y)});
}