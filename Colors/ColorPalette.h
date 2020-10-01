#pragma once

#include <algorithm>
namespace Colors
{
template<typename EntryT>
struct ColorPalette
{
	std::vector<EntryT> entries;

	ColorPalette() = default;
	ColorPalette(const ColorPalette<EntryT>& other) = default;

	template<typename OtherT>
	ColorPalette(const ColorPalette<OtherT>& other)
	{
		entries.resize(other.size());
		std::transform(other.entries.begin(), other.entries.end(), entries.begin(), [](const OtherT& color){ return colorspace_cast<EntryT>(color); });
	};

	size_t size() const{ return entries.size(); };

	template<typename Func>
	auto analyze(Func axis_func) -> std::pair<decltype(axis_func(EntryT())), decltype(axis_func(EntryT()))>
	{
		using AxisT = decltype(axis_func(EntryT()));
		constexpr auto NumAxis = std::tuple_size<AxisT>::value;
		std::vector<AxisT> axis;
		std::transform(entries.begin(), entries.end(), axis.begin(), axis_func);
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

	template<typename Func>
	void reduce_using_median_split(int max_colors, Func axis_func)
	{
		if(entries.size() <= max_colors)
			return;

		using AxisT = decltype(axis_func(EntryT()));

		constexpr auto NumAxis = std::tuple_size<AxisT>::value;

		struct EntryCopy
		{
			EntryT entry;
			AxisT axis;
		};
		using IteratorT = typename std::vector<EntryCopy>::iterator;

		struct Split
		{
			IteratorT from;
			IteratorT to;
			float axis_difference = -INFINITY;
			int sorted_by_axis = -1;
		};

		std::vector<EntryCopy> working_copy;
		std::vector<Split> splits;

		working_copy.resize(entries.size());
		splits.reserve(max_colors); 
		std::transform(entries.begin(), entries.end(), working_copy.begin(), [axis_func](const EntryT& val){ return EntryCopy{val, axis_func(val)}; });

		AxisT axis_minimum;
		AxisT axis_maximum;
		std::array<float, NumAxis> axis_difference_compensation;
		for(int i = 0; i < NumAxis; i++)
		{
			auto comp = [i](const EntryCopy& a, const EntryCopy& b){ return a.axis[i] < b.axis[i]; };
			const auto pair = std::minmax_element(working_copy.begin(), working_copy.end(), comp);
			axis_minimum[i] = pair.first->axis[i];
			axis_maximum[i] = pair.second->axis[i];
			axis_difference_compensation[i] = 1.0 / std::max(axis_maximum[i] - axis_minimum[i], 0.10f);
		}

		int micro_splits = 0;
		
		auto reduce_split = [&micro_splits, max_colors](Split& split)
		{
			int iterator_diff = std::distance(split.from, split.to);
			if((iterator_diff <= 1) || split.axis_difference < (0.05+(1.0 / max_colors))/2.0)
			{
				split.sorted_by_axis = -1;
				split.axis_difference = -INFINITY;
				split.to = split.from;
				micro_splits += 1;
			}
		};

		auto sort_by_axis = [](Split& s, int original_axis = -1)
		{
			if(s.sorted_by_axis != original_axis && s.sorted_by_axis != -1)
				std::sort(s.from, s.to, [&s](const EntryCopy& a, const EntryCopy& b){ return a.axis[s.sorted_by_axis] < b.axis[s.sorted_by_axis]; });
		};

		auto find_axis_and_sort = [axis_difference_compensation, sort_by_axis](Split& s)
		{
			int original_axis = s.sorted_by_axis;
			s.axis_difference = -INFINITY;
			s.sorted_by_axis = -1;
			for(int i = 0; i < NumAxis; i++)
			{
				auto comp = [i](const EntryCopy& a, const EntryCopy& b){ return a.axis[i] < b.axis[i]; };
				const auto pair = std::minmax_element(s.from, s.to, comp);
				float difference = (pair.second->axis[i] - pair.first->axis[i]) * axis_difference_compensation[i];
				if(difference > s.axis_difference)
				{
					s.axis_difference = difference;
					s.sorted_by_axis = i;
				}
			}
			sort_by_axis(s, original_axis);
		};

		splits.emplace_back();
		splits.back().from = working_copy.begin();
		splits.back().to = working_copy.end();
		find_axis_and_sort(splits.back());

		while((splits.size()*2 - micro_splits) < max_colors)
		{
			Split& to_split = *(std::max_element(splits.begin(), splits.end(), [](const Split& a, const Split& b){ return a.axis_difference < b.axis_difference; }));
			Split new_split = to_split;
			int iterator_diff = std::distance(to_split.from, to_split.to);
			auto split_point = to_split.from + iterator_diff / 2;
			to_split.to = split_point;
			new_split.from = split_point;
			find_axis_and_sort(to_split);
			find_axis_and_sort(new_split);
			reduce_split(to_split);
			reduce_split(new_split);
			splits.push_back(new_split);
		}

		std::sort(splits.begin(), splits.end(), [](const Split& a, const Split& b){ return a.axis_difference < b.axis_difference; });
		auto it = splits.rbegin();
		while((splits.size()*2 - micro_splits) > max_colors)
		{
			Split& to_reduce = *std::find_if(it, splits.rend(), [](const Split& val){ return val.axis_difference >= 0.0;});
			to_reduce.sorted_by_axis = -1;
			to_reduce.axis_difference = -INFINITY;
			to_reduce.to = to_reduce.from;
			micro_splits += 1;
		}

		std::vector<EntryT> result;
		result.reserve(max_colors);
		for(Split& split : splits)
		{
			IteratorT most_extreme;
			float most_extreme_extremeness = 0.0;
			int most_extreme_axis = 0;
			for(int i = 0; i < 5; i++)
			{
				auto comp = [i](const EntryCopy& a, const EntryCopy& b){ return a.axis[i] < b.axis[i]; };
				const auto pair = std::minmax_element(split.from, split.to, comp);
				float lowest = pair.first->axis[i];
				float highest = pair.second->axis[i];
				float minimum_extremeness = 1.0-(lowest - axis_minimum[i])/(axis_maximum[i] - axis_minimum[i]);
				float maximum_extremeness = (highest - axis_minimum[i])/(axis_maximum[i] - axis_minimum[i]);
				float extremeness = 0.0;
				IteratorT extreme_val;
				if(minimum_extremeness < maximum_extremeness)
				{
					extreme_val = pair.first;
					extremeness = minimum_extremeness;
				}
				else
				{
					extreme_val = pair.second;
					extremeness = maximum_extremeness;
				}
				if(i == 0) // Prioritize Luminance
					extremeness *= 2.0;
				
				if(extremeness > most_extreme_extremeness)
				{
					most_extreme = extreme_val;
					most_extreme_extremeness = extremeness;
					most_extreme_axis = i;
				}
			}
			split.sorted_by_axis = most_extreme_axis;
			sort_by_axis(split);
			result.push_back(split.from->entry);
			if(std::distance(split.from, split.to) > 1)
				result.push_back((split.to-1)->entry);
		}
		entries = std::move(result);
	}


};

}