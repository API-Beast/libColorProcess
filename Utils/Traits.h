#pragma once

template <typename T> struct type_of_member_variable;
template <typename R, typename C> struct type_of_member_variable<R C::*> { using type = R; };

template<typename T> inline constexpr T logical_max_value = std::numeric_limits<T>::max();
template<> inline constexpr float logical_max_value<float> = 1.0f;
template<> inline constexpr double logical_max_value<double> = 1.0;