#pragma once
#include "Palette.h"

#include <array>
#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>

template<typename EntryT, typename Func>
ColorPalette<EntryT> Palette::sort(const ColorPalette<EntryT>& pal, Func stat_function, std::initializer_list<int> fixed_indices)
{
	std::vector<EntryT> result = pal;
	int length = result.size();
	if(length < 3)
		return result;

	std::vector<bool> is_fixed(length, false);
	std::vector<bool> is_set(length, false);

	using StatT = decltype(stat_function(EntryT()));
	constexpr auto NumAxis = std::tuple_size<StatT>::value;
	
	std::vector<StatT> stats(length);
	std::transform(result.begin(), result.end(), stats.begin(), stat_function);

	auto get_adjacent_spaces = [](int i)->std::array<int,2>{ return {i-1, i+1}; };
	auto is_space_valid = [length, &is_fixed](int i){ return i > 0 && i < length;};
	auto is_space_invalid = [is_space_valid](int i){ return !is_space_valid(i);};
	auto is_space_occupied = [is_space_valid, &is_set](int i){ return !is_space_valid(i) || is_set[i]; };

	std::vector<int> open_spaces;
	for(int index : fixed_indices)
	{
		is_fixed[index] = true;
		is_set[index] = true;
		auto spaces = get_adjacent_spaces(index);
		open_spaces.insert(open_spaces.end(), spaces.begin(), spaces.end());
	}

	// We need at least one fixed index to start sorting
	if(fixed_indices.size() < 1)
	{
		// This is all for finding the color median on the first axis (we just assume the first axis is the most significant)
		std::vector<int> proxy(length);
		std::iota(proxy.begin(), proxy.end(), 0);
		auto middle = proxy.begin() + length / 2;
		std::nth_element(proxy.begin(), middle, proxy.end(), [&stats](int a, int b){ return stats[a][0] < stats[b][0]; });
	
		int index = length / 2;
		is_fixed[index] = true;
		is_set[index] = true;
		std::swap(stats[index], stats[*middle]);
		std::swap(result[index], result[*middle]);
		auto spaces = get_adjacent_spaces(index);
		open_spaces.insert(open_spaces.end(), spaces.begin(), spaces.end());
	}

	auto calc_error = [&](const StatT& entry, int position)
	{
		const int depth = 3;
		float distance = 0.0;

		for(int i = 1; i <= depth; i++)
		{
			auto calc_distance = [&](int pos)
			{
				if(!is_space_valid(pos)) return 0.0f;
				if(!is_set[pos]) return 0.0f;
				float temp = 0.0;
				for(int axis = 0; axis<NumAxis; axis++)
					temp += std::pow(entry[axis] - stats[pos][axis], 2.0f);
				return temp;
			};

			distance += calc_distance(position - i) * (1.0 - (i-1) / float(depth));
			distance += calc_distance(position + i) * (1.0 - (i-1) / float(depth));
		}
		return distance;
	};

	std::vector<int> best_entries;
	std::vector<float> entry_distance;
	best_entries.reserve(fixed_indices.size()*2);
	entry_distance.reserve(fixed_indices.size()*2);
	while(true)
	{
		open_spaces.erase(std::remove_if(open_spaces.begin(), open_spaces.end(), is_space_occupied), open_spaces.end());
		// Remove duplicates
		std::sort(open_spaces.begin(), open_spaces.end());
		open_spaces.erase(std::unique(open_spaces.begin(), open_spaces.end()), open_spaces.end());

		if(open_spaces.empty())
			break;

		best_entries.resize(open_spaces.size());
		entry_distance.resize(open_spaces.size());
		std::fill(best_entries.begin(), best_entries.end(), -1);
		std::fill(entry_distance.begin(), entry_distance.end(), INFINITY);

		for(int i = 0; i < open_spaces.size(); i++)
		for(int j = 0; j < result.size(); j++)
		{
			if(!is_set[j])
			{
				float distance = calc_error(stats[j], open_spaces[i]);
				if(distance < entry_distance[i])
				{
					best_entries[i] = j;
					entry_distance[i] = distance;
				}
			}
		}

		int best_substitution = 0;
		for(int i = 1; i < open_spaces.size(); i++)
			if(entry_distance[i] < entry_distance[best_substitution])
				best_substitution = i;

		int index = open_spaces[best_substitution];
		int entry = best_entries[best_substitution];
		is_set[index] = true;

		std::swap(stats[index], stats[entry]);
		std::swap(result[index], result[entry]);
		auto spaces = get_adjacent_spaces(index);
		open_spaces.insert(open_spaces.end(), spaces.begin(), spaces.end());
	}
	return result;
};