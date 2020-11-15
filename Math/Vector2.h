#pragma once

#include <cmath>

template<typename T>
struct Vector2
{
	T x;
	T y;

	constexpr Vector2() = default;
	constexpr Vector2(T scalar):x(scalar),y(scalar){};
	constexpr Vector2(T px, T py):x(px),y(py){};

	template<typename P>
	constexpr Vector2(Vector2<P> other) : x(other.x), y(other.y){};
	
	constexpr bool operator==(const Vector2<T>& other){ return x == other.x && y == other.y;  };
	constexpr bool operator!=(const Vector2<T>& other){ return !operator==(other);  };
	constexpr Vector2<T> operator-(){ return Vector2<T>(-x, -y); };

	constexpr float square_length(){ return (x * x) + (y * y); };
	constexpr float length(){ return std::sqrt(square_length()); };

	constexpr Vector2<T> normalized(){ return length() > 0 ? ((*this) / length()) : Vector2<T>(); };

	template<typename Func> constexpr Vector2<T> visit(Func f){ return Vector2<T>(f(x), f(y));};
	template<typename Func> constexpr Vector2<T> visit(Func f, Vector2<T> a)
	{
		return Vector2<T>(f(x, a.x), f(y, a.y));
	};
	template<typename Func> constexpr Vector2<T> visit(Func f, Vector2<T> a, Vector2<T> b)
	{
		return Vector2<T>(f(x, a.x, y.x), f(y, a.y, b.y));
	};
	template<typename Func> constexpr Vector2<T> visit(Func f, Vector2<T> a, Vector2<T> b, Vector2<T> c)
	{
		return Vector2<T>(f(x, a.x, b.x, c.x), f(y, a.y, b.y, c.y));
	};

	constexpr Vector2<T> clamp(Vector2<T> min, Vector2<T> max)
	{
		return visit([](auto val, auto min, auto max)
		{
			if(val < min) return min;
			if(val > max) return max;
			return val;
		}, min, max);
	};
};

using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;
using Vec2u8 = Vector2<unsigned char>;

#define def_operator(OPER)\
/* vector x vector */\
template<typename T, typename P>\
constexpr auto operator OPER(const Vector2<T>& vec, const Vector2<P>& other)\
-> Vector2<decltype(P() OPER T())>\
{\
	return {vec.x OPER other.x, vec.y OPER other.y};\
}\
/* vector x scalar */\
template<typename T>\
constexpr auto operator OPER(const Vector2<T>& vec, float scalar)\
 -> Vector2<decltype(T() OPER float())>\
{\
	return {vec.x OPER scalar, vec.y OPER scalar};\
}\
/* scalar x vector */\
template<typename T>\
constexpr auto operator OPER(float scalar, const Vector2<T>& vec)\
 -> Vector2<decltype(float() OPER T())>\
{\
	return {scalar OPER vec.x, scalar OPER vec.y};\
}
def_operator(+);
def_operator(-);
def_operator(/);
def_operator(*);
def_operator(%);
#undef def_operator

#define def_assign_operator(OPER)\
/* vector x vector */\
template<typename T, typename P>\
Vector2<T>& operator OPER(Vector2<T>& vec, const Vector2<P>& other){ vec.x OPER other.x; vec.y OPER other.y; return vec; }\
/* vector x scalar */\
template<typename T>\
Vector2<T>& operator OPER(Vector2<T>& vec, float scalar){ vec.x OPER scalar; vec.y OPER scalar; return vec; }
def_assign_operator(+=);
def_assign_operator(-=);
def_assign_operator(/=);
def_assign_operator(*=);
def_assign_operator(%=);
#undef def_assign_operator