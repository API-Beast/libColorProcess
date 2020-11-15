#pragma once

#include "../Colors/ColorSpaces.h"
#include "../Colors/Stats.h"
#include <vector>
#include <algorithm>
#include "../Image/ImageData.h"
#include <iterator>

template<typename T>
using ColorPalette = std::vector<T>;

namespace Palette
{
	template<typename EntryT, typename It>
	ColorPalette<EntryT> import_from_image(It begin, It end);
	template<typename EntryT>
	ColorPalette<EntryT> import_from_image(const ImageData<EntryT>& data);

	template<typename To, typename From>
	[[nodiscard]] ColorPalette<To> convert(const ColorPalette<From>& src);
	
	template<typename EntryT, typename StatsFn>
	[[nodiscard]] ColorPalette<EntryT> reduce_using_median_split(const ColorPalette<EntryT>& pal, int max_colors, StatsFn axis_func);

	template<typename EntryT, typename StatsFn>
	[[nodiscard]] ColorPalette<EntryT> sort(const ColorPalette<EntryT>& pal, StatsFn axis_func, std::initializer_list<int> fixed_indices = {});

	template<typename EntryT, typename StatsFn, typename StatsT>
	[[nodiscard]] EntryT map_color_relative(const ColorPalette<EntryT>& pal, EntryT input, StatsFn axis_func, const StatsT& paletteStats, const StatsT& sourceStats);

	template<typename EntryT>
	[[nodiscard]] EntryT map_color_absolute(const ColorPalette<EntryT>& pal, EntryT input);

	template<typename EntryT, typename StatsFn, typename StatsT>
	[[nodiscard]] EntryT map_color_relative_multisample(const ColorPalette<EntryT>& pal, EntryT input, StatsFn analysis_function, const std::pair<StatsT, StatsT>& paletteStats, const std::pair<StatsT, StatsT>& sourceStats, StatsT weights = Stats::identity< std::tuple_size_v<StatsT> >());
}

template<typename To, typename From>
ColorPalette<To> Palette::convert(const ColorPalette<From>& src)
{
	ColorPalette<To> retVal(src.size());
	std::transform(src.begin(), src.end(), retVal.begin(), [](const From& color){ return colorspace_cast<To>(color); });
	return retVal;
};

template<typename EntryT>
ColorPalette<EntryT> Palette::import_from_image(const ImageData<EntryT>& data) 
{
	return Palette::import_from_image<EntryT>(data.begin(), data.end());
}

template<typename EntryT, typename It>
ColorPalette<EntryT> Palette::import_from_image(It begin, It end)
{
	ColorPalette<EntryT> retVal;
	retVal.assign(begin, end);
	std::sort(retVal.begin(), retVal.end());
	retVal.erase(std::unique(retVal.begin(), retVal.end()), retVal.end());
	return retVal;
}


#include "Palette_MedianSplit.hpp"
#include "Palette_Sort.hpp"
#include "Palette_MapColor.hpp"
