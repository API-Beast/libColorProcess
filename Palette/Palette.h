#pragma once

#include <vector>
#include <algorithm>

template<typename T>
using ColorPalette = std::vector<T>;

namespace Palette
{
	template<typename To, typename From>
	ColorPalette<To> convert(const ColorPalette<From>& src);

	template<typename EntryT, typename Func>
	auto minmax_axis(const ColorPalette<EntryT>& src, Func axis_func) -> std::pair<decltype(axis_func(EntryT())), decltype(axis_func(EntryT()))>;

	template<typename EntryT, typename Func>
	ColorPalette<EntryT> reduce_using_median_split(const ColorPalette<EntryT>& pal, int max_colors, Func axis_func);

	template<typename EntryT, typename Func>
	ColorPalette<EntryT> sort(const ColorPalette<EntryT>& pal, Func analysis_function, std::initializer_list<int> fixed_indices = {});

	/*template<typename EntryT, typename Stats, typename Fn>
	EntryT map_color_relative(const ColorPalette<EntryT>& pal, const EntryT& input, const Stats& paletteStats, const Stats& sourceStats, Fn manipulator = nullptr);

	template<typename EntryT>
	EntryT map_color_absolute(const ColorPalette<EntryT>& pal, const EntryT& input);*/
}

template<typename To, typename From>
ColorPalette<To> Palette::convert(const ColorPalette<From>& src)
{
	ColorPalette<To> retVal(src.size());
	std::transform(src.begin(), src.end(), retVal.begin(), [](const From& color){ return colorspace_cast<To>(color); });
	return retVal;
};

template<typename EntryT, typename Func>
auto Palette::minmax_axis(const ColorPalette<EntryT>& src, Func axis_func) -> std::pair<decltype(axis_func(EntryT())), decltype(axis_func(EntryT()))>
{
	using AxisT = decltype(axis_func(EntryT()));
	constexpr auto NumAxis = std::tuple_size<AxisT>::value;
	std::vector<AxisT> axis;
	std::transform(src.begin(), src.end(), axis.begin(), axis_func);
	std::pair<AxisT, AxisT> retVal;
	for(int i = 0; i < NumAxis; i++)
	{
		auto comp = [i](const AxisT& a, const AxisT& b){ return a[i] < b[i]; };
		const auto pair = std::minmax_element(axis.begin(), axis.end(), comp);
		retVal[i] = (*pair.first)[i];
		retVal[i] = (*pair.second)[i];
	}
	return retVal;
}

#include "Palette_MedianSplit.hpp"
#include "Palette_Sort.hpp"
