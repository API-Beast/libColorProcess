#pragma once
#include <cmath>
#include <type_traits>
#include <tuple>

#define VECTOR3_CONSTRUCTORS(ClassName, x, y, z)\
constexpr ClassName()=default;\
constexpr ClassName(float scalar){ x = scalar; y = scalar; z = scalar;};\
constexpr ClassName(decltype(x) px, decltype(y) py, decltype(z) pz)\
{\
	x = px;\
	y = py;\
	z = pz;\
};

#define VECTOR3_MEMBER_FUNCTIONS(ClassName, x, y, z) \
using VectorBaseT = typename std::remove_reference<decltype(bool() ? x : y ? y : z)>::type;\
constexpr static bool is_vector3_type = true;\
constexpr static bool is_vector_type = true;\
constexpr static int num_components = 3;\
constexpr bool operator==(const ClassName& other){ return x == other.x && y == other.y && z == other.z;  };\
constexpr bool operator!=(const ClassName& other){ return !operator==(other);  };\
constexpr bool operator<(const ClassName& other)\
{\
	return x == other.x ? (y == other.y ? z < other.z : y < other.y) : x < other.x;\
};\
constexpr bool operator>(const ClassName& other){ return !operator<=(other);  };\
constexpr bool operator>=(const ClassName& other){ return !operator<(other);  };\
constexpr bool operator<=(const ClassName& other){ return operator<(other) || operator==(other);  };\
constexpr ClassName operator-() const{ return ClassName(-(x), -(y), -(z)); };\
\
constexpr float square_length(){ return ((x) * (x)) + ((y) * (y)) + ((z) * (z)); };\
constexpr float length(){ return std::sqrt(square_length()); };\
\
constexpr ClassName normalized(){ float l = length(); return l > 0.0f ? ClassName{static_cast<decltype(x)>(x/l), static_cast<decltype(y)>(y/l), static_cast<decltype(z)>(z/l)} : ClassName(); };\
\
constexpr VectorBaseT max_value(){ return x > y ? x : (y > z ? y : z); };\
constexpr VectorBaseT min_value(){ return x < y ? x : (y < z ? y : z); };\
constexpr decltype(x) get_x() const{ return x; };\
constexpr decltype(y) get_y() const{ return y; };\
constexpr decltype(z) get_z() const{ return z; };\
\
template<typename Func> constexpr ClassName visit(Func f) const{ return ClassName(f(x), f(y), f(z));};\
template<typename Func> constexpr ClassName visit(Func f, ClassName a) const{ return ClassName(f(x, a.x), f(y, a.y), f(z, a.z));};\
template<typename Func> constexpr ClassName visit(Func f, ClassName a, ClassName b) const{ return ClassName(f(x, a.x, b.x), f(y, a.y, b.y), f(z, a.z, b.z));};\
template<typename Func> constexpr ClassName visit(Func f, ClassName a, ClassName b, ClassName c) const{ return ClassName(f(x, a.x, b.x, c.x), f(y, a.y, b.y, c.y), f(z, a.z, b.z, c.z));};\
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
#define VECTOR3_OPERATOR(ClassName, OPER, x, y, z, template_args)\
/* vector x vector */\
template_args \
constexpr ClassName operator OPER(const ClassName& vec, const ClassName& other){ return ClassName(vec.x OPER other.x, vec.y OPER other.y, vec.z OPER other.z); };\
/* vector x scalar */\
template_args \
constexpr ClassName operator OPER(const ClassName& vec, typename ClassName::VectorBaseT scalar){ return ClassName(vec.x OPER scalar, vec.y OPER scalar, vec.z OPER scalar); };\
/* scalar x vector */\
template_args \
constexpr ClassName operator OPER(typename ClassName::VectorBaseT scalar, const ClassName& vec){ return ClassName(static_cast<decltype(vec.x)>(scalar OPER vec.x), static_cast<decltype(vec.x)>(scalar OPER vec.y), static_cast<decltype(vec.z)>(scalar OPER vec.z)); };

// TODO: These can be moved inside the class.
#define VECTOR3_ASSIGN_OPERATOR(ClassName, OPER, x, y, z, template_args)\
/* vector x vector */\
template_args \
inline ClassName& operator OPER(ClassName& vec, const ClassName& other){ vec.x OPER other.x; vec.y OPER other.y; vec.z OPER other.z; return vec; };\
/* vector x scalar */\
template_args \
inline ClassName& operator OPER(ClassName& vec, typename ClassName::VectorBaseT scalar){ vec.x OPER scalar; vec.y OPER scalar; vec.z OPER scalar; return vec; };

#define VECTOR3_OPERATORS_T(ClassName, x, y, z, template_args)\
VECTOR3_OPERATOR(ClassName, +, x, y, z, template_args);\
VECTOR3_OPERATOR(ClassName, -, x, y, z, template_args);\
VECTOR3_OPERATOR(ClassName, /, x, y, z, template_args);\
VECTOR3_OPERATOR(ClassName, *, x, y, z, template_args);\
VECTOR3_ASSIGN_OPERATOR(ClassName, +=, x, y, z, template_args);\
VECTOR3_ASSIGN_OPERATOR(ClassName, -=, x, y, z, template_args);\
VECTOR3_ASSIGN_OPERATOR(ClassName, /=, x, y, z, template_args);\
VECTOR3_ASSIGN_OPERATOR(ClassName, *=, x, y, z, template_args);

#define VECTOR3_OPERATORS(ClassName, x, y, z) VECTOR3_OPERATORS_T(ClassName, x, y, z,)