#pragma once

#include "ColorSpaces.h"
#include "Colors.h"
#include <array>
#include <vector>
#include <iterator>
#include <algorithm>
#include <utility>
#include <type_traits>

namespace Stats
{

inline std::array<float, 5> perceptive_factors(LinearRGB color)
{
	return {Colors::luminance709(color), Colors::vibrance(color), Colors::redness(color), Colors::greenness(color), Colors::blueness(color)};
};

constexpr std::array<float, 3> linrgb_factors(LinearRGB color)
{
	return {color.red, color.green, color.blue};
};

constexpr std::array<float, 3> srgb_factors(sRGB color)
{
	return {color.red, color.green, color.blue};
};

template<int Size>
constexpr std::array<float, Size> identity()
{
	std::array<float, Size> retVal;
	for(int i = 0; i < Size; i++)
		retVal[i] = 1.0;
	return retVal;
};

// TODO: Replace std::pair with special struct
template<typename It, typename Func, typename EntryT = typename std::iterator_traits<It>::value_type>
auto gather_stat_range(It first, It last, Func axis_func) -> std::pair< decltype(axis_func(EntryT())), decltype(axis_func(EntryT())) >
{
	using AxisT = decltype(axis_func(EntryT()));
	constexpr auto NumAxis = std::tuple_size<AxisT>::value;
	std::vector<AxisT> axis;
	axis.resize(std::distance(first, last));
	std::transform(first, last, axis.begin(), axis_func);
	std::pair<AxisT, AxisT> retVal;
	for(int i = 0; i < NumAxis; i++)
	{
		auto comp = [i](const AxisT& a, const AxisT& b){ return a[i] < b[i]; };
		const auto pair = std::minmax_element(axis.begin(), axis.end(), comp);
		retVal.first[i] = (*pair.first)[i];
		retVal.second[i] = (*pair.second)[i];
	}
	return retVal;
};


}