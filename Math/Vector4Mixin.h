#pragma once

#include <cmath>
#include <type_traits>
#include <cstdint>
#include <limits>

#include "../Utils/Traits.h"

template<typename T, auto X, auto Y, auto Z, auto W>
struct Vector4Mixin : public T
{
	using SelfT = Vector4Mixin<T, X, Y, Z, W>;
	using X_Type = typename mem_type<decltype(X)>::type;
	using Y_Type = typename mem_type<decltype(Y)>::type;
	using Z_Type = typename mem_type<decltype(Z)>::type;
	using W_Type = typename mem_type<decltype(W)>::type;
	using BaseType = T;
	static_assert(X != Y, "Possible typo, X and Y must point to different members");
	static_assert(Y != Z, "Possible typo, Y and Z must point to different members");
	static_assert(Z != W, "Possible typo, X and Y must point to different members");
	static_assert(X != W, "Possible typo, X and Y must point to different members");

	constexpr Vector4Mixin() : T(){};
	constexpr Vector4Mixin(const T& base) : T(base){};
	constexpr Vector4Mixin(float scalar) : T(){ this->*X = scalar; this->*Y = scalar; this->*Z = scalar; this->*W = scalar; };
	
	template<typename T2, auto X2, auto Y2, auto Z2>
	constexpr Vector4Mixin(const Vector3Mixin<T2, X2, Y2, Z2>& other, W_Type pw = logical_max_value<T>)
	{
		this->*X = other.*X2;
		this->*Y = other.*Y2;
		this->*Z = other.*Z2;
		this->*W = pw;
	};

	constexpr Vector4Mixin(X_Type px, Y_Type py, Z_Type pz, W_Type pw = logical_max_value<T>) : T()
	{
		this->*X = px;
		this->*Y = py;
		this->*Z = pz;
		this->*W = pw;
	};

	template <typename P, auto... Args>
	constexpr Vector4Mixin(const Vector4Mixin<P, Args...>& p) : T(p){};
	
	constexpr bool operator==(const SelfT& other){ return this->*X == other.*X && this->*Y == other.*Y && this->*Z == other.*Z && this->*W == other.*W;  };
	constexpr bool operator!=(const SelfT& other){ return !operator==(other);  };
	constexpr SelfT operator-() const{ return SelfT(-(this->*X), -(this->*Y), -(this->*Z), -(this->*W)); };

	constexpr float squareLength(){ return ((this->*X) * (this->*X)) + ((this->*Y) * (this->*Y)) + ((this->*Z) * (this->*Z)) + ((this->*W) * (this->*W)); };
	constexpr float length(){ return std::sqrt(squareLength()); };

	constexpr SelfT normalized(){ return length() > 0 ? ((*this) / length()) : SelfT(); };

	template<typename Func> constexpr SelfT visit(Func f){ return SelfT(f(this->*X), f(this->*Y), f(this->*Z), f(this->*W));};
	template<typename Func> constexpr void for_each(Func f){ f(this->*X); f(this->*Y); f(this->*Z); f(this->*W); };
};

#define def_operator(OPER)\
/* vector x vector */\
template<typename T, auto X, auto Y, auto Z, auto W, typename P, auto X2, auto Y2, auto Z2, auto W2>\
constexpr Vector4Mixin<T, X, Y, Z, W> operator OPER(const Vector4Mixin<T, X, Y, Z, W>& vec, const Vector4Mixin<P, X2, Y2, Z2, W2>& other){ Vector4Mixin<T, X, Y, Z, W> converted(other); return {vec.*X OPER converted.*X, vec.*Y OPER converted.*Y, vec.*Z OPER converted.*Z, vec.*W OPER converted.*W}; }\
/* vector x scalar */\
template<typename T, auto X, auto Y, auto Z, auto W>\
constexpr Vector4Mixin<T, X, Y, Z, W> operator OPER(const Vector4Mixin<T, X, Y, Z, W>& vec, float scalar){ return {vec.*X OPER scalar, vec.*Y OPER scalar, vec.*Z OPER scalar, vec.*W OPER scalar}; }\
/* scalar x vector */\
template<typename T, auto X, auto Y, auto Z, auto W>\
constexpr Vector4Mixin<T, X, Y, Z, W> operator OPER(float scalar, const Vector4Mixin<T, X, Y, Z, W>& vec){ return {scalar OPER vec.*X, scalar OPER vec.*Y, scalar OPER vec.*Z, scalar OPER vec.*W}; }
def_operator(+);
def_operator(-);
def_operator(/);
def_operator(*);
def_operator(%);
#undef def_operator

#define def_assign_operator(OPER)\
/* vector x vector */\
template<typename T, auto X, auto Y, auto Z, auto W, typename P, auto X2, auto Y2, auto Z2, auto W2>\
Vector4Mixin<T, X, Y, Z, W>& operator OPER(Vector4Mixin<T, X, Y, Z, W>& vec, const Vector4Mixin<P, X2, Y2, Z2, W2>& other){ vec.*X OPER other.*X2; vec.*Y OPER other.*Y2; vec.*Z OPER other.*Z2; vec.*W OPER other.*W2; return vec; }\
/* vector x scalar */\
template<typename T, auto X, auto Y, auto Z, auto W>\
Vector4Mixin<T, X, Y, Z, W>& operator OPER(Vector4Mixin<T, X, Y, Z, W>& vec, float scalar){ vec.*X OPER scalar; vec.*Y OPER scalar; vec.*Z OPER scalar; vec.*W OPER scalar; return vec; }
def_assign_operator(+=);
def_assign_operator(-=);
def_assign_operator(/=);
def_assign_operator(*=);
def_assign_operator(%=);
#undef def_assign_operator