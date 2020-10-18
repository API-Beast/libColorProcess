#pragma once
#include "yestest.h"
#include "../Math/Vector3Mixin.h"
#include "../Math/Vector2.h"

namespace YesTest
{
	template<typename T, auto X, auto Y, auto Z>
	void print_value(Vector3Mixin<T,X,Y,Z> f) { print_value(f.*X, f.*Y, f.*Z); }

	template<typename T, auto X, auto Y, auto Z>
	inline int ulp_difference(Vector3Mixin<T,X,Y,Z> a, Vector3Mixin<T,X,Y,Z> b)
	{
		return std::max({ulp_difference(a.*X, b.*X), ulp_difference(a.*Y, b.*Y), ulp_difference(a.*Z, b.*Z)});
	}

	template<typename T>
	void print_value(Vector2<T> f) { print_value(f.x, f.y); }

	template<typename T>
	inline int ulp_difference(Vector2<T> a, Vector2<T> b)
	{
		return std::max({ulp_difference(a.x, b.x), ulp_difference(a.y, b.y)});
	}
}