#pragma once
#include <cmath>
#include <type_traits>
#include <tuple>
#include "../Utils/Traits.h"

#define VECTOR4_CONSTRUCTORS(ClassName, x, y, z, w)\
constexpr ClassName()=default;\
constexpr ClassName(float scalar){ x = scalar; y = scalar; z = scalar; w = scalar; };\
constexpr ClassName(decltype(x) px, decltype(y) py, decltype(z) pz, decltype(w) pw)\
{\
	x = px;\
	y = py;\
	z = pz;\
	w = pw;\
};

#define VECTOR4_MEMBER_FUNCTIONS(ClassName, x, y, z, w) \
using VectorBaseT = typename std::remove_reference<decltype(bool() ? x : y ? y : z ? z : w)>::type;\
constexpr static bool is_vector4_type = true;\
constexpr static bool is_vector_type = true;\
constexpr static int num_components = 4;\
constexpr bool operator==(const ClassName& other){ return x == other.x && y == other.y && z == other.z && w == other.w;  };\
constexpr bool operator!=(const ClassName& other){ return !operator==(other);  };\
constexpr bool operator<(const ClassName& other)\
{\
	return x == other.x ? (y == other.y ? (z == other.z ? w < other.w : z < other.z) : y < other.y) : x < other.x;\
};\
constexpr bool operator>(const ClassName& other){ return !operator<=(other);  };\
constexpr bool operator>=(const ClassName& other){ return !operator<(other);  };\
constexpr bool operator<=(const ClassName& other){ return operator<(other) || operator==(other);  };\
constexpr ClassName operator-() const{ return ClassName(-(x), -(y), -(z), -(w)); };\
\
constexpr float square_length(){ return ((x) * (x)) + ((y) * (y)) + ((z) * (z)) + ((w) * (w)); };\
constexpr float length(){ return std::sqrt(square_length()); };\
\
constexpr ClassName normalized(){ float l = length(); return l > 0.0f ? ClassName{static_cast<decltype(x)>(x/l), static_cast<decltype(y)>(y/l), static_cast<decltype(z)>(z/l), static_cast<decltype(w)>(w/l)} : ClassName(); };\
\
constexpr VectorBaseT max_value(){ return x > y ? x : (y > z ? y : (z > w ? z : w)); };\
constexpr VectorBaseT min_value(){ return x < y ? x : (y < z ? y : (z < w ? z : w)); };\
constexpr decltype(x) get_x() const{ return x; };\
constexpr decltype(y) get_y() const{ return y; };\
constexpr decltype(z) get_z() const{ return z; };\
constexpr decltype(w) get_w() const{ return w; };\
template<typename Func> constexpr ClassName visit(Func f) const{ return ClassName(f(x), f(y), f(z), f(w));};\
template<typename Func> constexpr ClassName visit(Func f, ClassName a) const{ return ClassName(f(x, a.x), f(y, a.y), f(z, a.z), f(w, a.w));};\
template<typename Func> constexpr ClassName visit(Func f, ClassName a, ClassName b) const{ return ClassName(f(x, a.x, b.x), f(y, a.y, b.y), f(z, a.z, b.z), f(w, a.w, b.w));};\
template<typename Func> constexpr ClassName visit(Func f, ClassName a, ClassName b, ClassName c) const{ return ClassName(f(x, a.x, b.x, c.x), f(y, a.y, b.y, c.y), f(z, a.z, b.z, c.z), f(w, a.w, b.w, c.w));};\
\
constexpr ClassName clamp(ClassName min, ClassName max)\
{\
	return visit([](auto val, auto min, auto max)\
	{\
		if(val < min) return min;\
		if(val > max) return max;\
		return val;\
	}, min, max);\
};

// TODO: All of these can be moved inside the class except scalar x vector.
#define VECTOR4_OPERATOR(ClassName, OPER, x, y, z, w, template_args)\
/* vector x vector */\
template_args \
constexpr ClassName operator OPER(const ClassName& vec, const ClassName& other){ return ClassName(vec.x OPER other.x, vec.y OPER other.y, vec.z OPER other.z, vec.w OPER other.w); };\
/* vector x scalar */\
template_args \
constexpr ClassName operator OPER(const ClassName& vec, typename ClassName::VectorBaseT scalar){ return ClassName(vec.x OPER scalar, vec.y OPER scalar, vec.z OPER scalar, vec.w OPER scalar); };\
/* scalar x vector */\
template_args \
constexpr ClassName operator OPER(typename ClassName::VectorBaseT scalar, const ClassName& vec){ return ClassName(static_cast<decltype(vec.x)>(scalar OPER vec.x), static_cast<decltype(vec.x)>(scalar OPER vec.y), static_cast<decltype(vec.z)>(scalar OPER vec.z), static_cast<decltype(vec.w)>(scalar OPER vec.w)); };

// TODO: These can be moved inside the class.
#define VECTOR4_ASSIGN_OPERATOR(ClassName, OPER, x, y, z, w, template_args)\
/* vector x vector */\
template_args \
inline ClassName& operator OPER(ClassName& vec, const ClassName& other){ vec.x OPER other.x; vec.y OPER other.y; vec.z OPER other.z; vec.w OPER other.w; return vec; };\
/* vector x scalar */\
template_args \
inline ClassName& operator OPER(ClassName& vec, typename ClassName::VectorBaseT scalar){ vec.x OPER scalar; vec.y OPER scalar; vec.z OPER scalar; vec.w OPER scalar; return vec; };

#define VECTOR4_OPERATORS_T(ClassName, x, y, z, w, template_args)\
VECTOR4_OPERATOR(ClassName, +, x, y, z, w, template_args);\
VECTOR4_OPERATOR(ClassName, -, x, y, z, w, template_args);\
VECTOR4_OPERATOR(ClassName, /, x, y, z, w, template_args);\
VECTOR4_OPERATOR(ClassName, *, x, y, z, w, template_args);\
VECTOR4_ASSIGN_OPERATOR(ClassName, +=, x, y, z, w, template_args);\
VECTOR4_ASSIGN_OPERATOR(ClassName, -=, x, y, z, w, template_args);\
VECTOR4_ASSIGN_OPERATOR(ClassName, /=, x, y, z, w, template_args);\
VECTOR4_ASSIGN_OPERATOR(ClassName, *=, x, y, z, w, template_args);

#define VECTOR4_OPERATORS(ClassName, x, y, z, w) VECTOR4_OPERATORS_T(ClassName, x, y, z, w,)