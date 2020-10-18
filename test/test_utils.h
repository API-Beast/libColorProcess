#pragma once
#include "yestest.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include <algorithm>

namespace YesTest
{
	template<typename T>
	typename std::enable_if<T::is_vector_type, void>::type print_value(T f) { f.for_each([](auto v){ print_value(v); }); }

	template<typename T>
	inline typename std::enable_if<T::is_vector_type, int>::type ulp_difference(T a, T b)
	{
		return std::max({ulp_difference(a.get_x(), b.get_x()), ulp_difference(a.get_y(), b.get_y()), ulp_difference(a.get_z(), b.get_z())});
	}

	template<typename T>
	void print_value(Vector2<T> f) { print_value(f.x, f.y); }

	template<typename T>
	inline int ulp_difference(Vector2<T> a, Vector2<T> b)
	{
		return std::max({ulp_difference(a.x, b.x), ulp_difference(a.y, b.y)});
	}
}