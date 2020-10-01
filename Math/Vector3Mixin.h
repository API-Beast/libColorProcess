#pragma once

#include <cmath>
#include <type_traits>

template <typename T> struct mem_type;
template <typename R, typename C> struct mem_type<R C::*> { using type = R; };

template<typename T, auto X, auto Y, auto Z>
struct Vector3Mixin : public T
{
	using SelfT = Vector3Mixin<T, X, Y, Z>;
	using X_Type = typename mem_type<decltype(X)>::type;
	using Y_Type = typename mem_type<decltype(Y)>::type;
	using Z_Type = typename mem_type<decltype(Z)>::type;
	static_assert(X != Y, "Possible typo, X and Y must point to different members");
	static_assert(Y != Z, "Possible typo, Y and Z must point to different members");

	constexpr Vector3Mixin() : T(){};
	constexpr Vector3Mixin(const T& base) : T(base){};
	constexpr Vector3Mixin(float scalar) : T(){ this->*X = scalar; this->*Y = scalar; this->*Z = scalar;};
	constexpr Vector3Mixin(X_Type px, Y_Type py, Z_Type pz) : T()
	{
		this->*X = px;
		this->*Y = py;
		this->*Z = pz;
	};

	template <typename P, auto... Args>
	constexpr Vector3Mixin(const Vector3Mixin<P, Args...>& p) : T(p){};
	
	constexpr bool operator==(const SelfT& other){ return this->*X == other.*X && this->*Y == other.*Y && this->*Z == other.*Z;  };
	constexpr bool operator!=(const SelfT& other){ return !operator==(other);  };
	constexpr SelfT operator-() const{ return SelfT(-(this->*X), -(this->*Y), -(this->*Z)); };

	constexpr float squareLength(){ return ((this->*X) * (this->*X)) + ((this->*Y) * (this->*Y)) + ((this->*Z) * (this->*Z)); };
	constexpr float length(){ return std::sqrt(squareLength()); };

	constexpr SelfT normalized(){ return length() > 0 ? ((*this) / length()) : SelfT(); };

	template<typename Func> constexpr SelfT visit(Func f){ return SelfT(f(this->*X), f(this->*Y), f(this->*Z));};
	template<typename Func> constexpr void for_each(Func f){ f(this->*X); f(this->*Y); f(this->*Z); };
};

#define def_operator(OPER)\
/* vector x vector */\
template<typename T, auto X, auto Y, auto Z, typename P, auto X2, auto Y2, auto Z2>\
constexpr Vector3Mixin<T, X, Y, Z> operator OPER(const Vector3Mixin<T, X, Y, Z>& vec, const Vector3Mixin<P, X2, Y2, Z2>& other){ Vector3Mixin<T, X, Y, Z> converted(other); return {vec.*X OPER converted.*X, vec.*Y OPER converted.*Y, vec.*Z OPER converted.*Z}; }\
/* vector x scalar */\
template<typename T, auto X, auto Y, auto Z>\
constexpr Vector3Mixin<T, X, Y, Z> operator OPER(const Vector3Mixin<T, X, Y, Z>& vec, float scalar){ return {vec.*X OPER scalar, vec.*Y OPER scalar, vec.*Z OPER scalar}; }\
/* scalar x vector */\
template<typename T, auto X, auto Y, auto Z>\
constexpr Vector3Mixin<T, X, Y, Z> operator OPER(float scalar, const Vector3Mixin<T, X, Y, Z>& vec){ return {scalar OPER vec.*X, scalar OPER vec.*Y, scalar OPER vec.*Z}; }
def_operator(+);
def_operator(-);
def_operator(/);
def_operator(*);
def_operator(%);
#undef def_operator

#define def_assign_operator(OPER)\
/* vector x vector */\
template<typename T, auto X, auto Y, auto Z, typename P, auto X2, auto Y2, auto Z2>\
Vector3Mixin<T, X, Y, Z>& operator OPER(Vector3Mixin<T, X, Y, Z>& vec, const Vector3Mixin<P, X2, Y2, Z2>& other){ vec.*X OPER other.*X2; vec.*Y OPER other.*Y2; vec.*Z OPER other.*Z2; return vec; }\
/* vector x scalar */\
template<typename T, auto X, auto Y, auto Z>\
Vector3Mixin<T, X, Y, Z>& operator OPER(Vector3Mixin<T, X, Y, Z>& vec, float scalar){ vec.*X OPER scalar; vec.*Y OPER scalar; vec.*Z OPER scalar; return vec; }
def_assign_operator(+=);
def_assign_operator(-=);
def_assign_operator(/=);
def_assign_operator(*=);
def_assign_operator(%=);
#undef def_operator