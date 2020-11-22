#pragma once
#include "Palette.h"
#include <cassert>
#include <cmath>

namespace Palette
{	
	template<typename EntryT>
	EntryT map_color_absolute(const ColorPalette<EntryT>& pal, EntryT input) 
	{
		assert(pal.size() > 0);
		float lowest_distance = HUGE_VALF;
		const EntryT* best_entry = &input;
		for(auto& entry : pal)
		{
			float distance = (input - entry).square_length();
			if(distance < lowest_distance)
			{
				best_entry = &entry;
				lowest_distance = distance;
			}
		}
		return *best_entry;
	}

	template<typename EntryT, typename StatsFn, typename StatsT>
	EntryT map_color_relative(const ColorPalette<EntryT>& pal, EntryT input, StatsFn analysis_function, const std::pair<StatsT, StatsT>& paletteStats, const std::pair<StatsT, StatsT>& sourceStats, StatsT weights = Stats::identity< std::tuple_size_v<StatsT> >())
	{
		assert(pal.size() > 0);
		static_assert(std::is_same_v<StatsT, std::decay_t<decltype(analysis_function(EntryT()))> >, "StatsT and return type of analysis_function needs to match.");
		constexpr auto N = std::tuple_size_v<StatsT>;
		using AxisType = typename StatsT::value_type;

		float lowest_distance = HUGE_VALF;
		const EntryT* best_entry = &input;
		auto inv_lerp = [](AxisType val, AxisType min, AxisType max)
		{
			return (val - min) / (max - min);
		};

		StatsT input_pos = analysis_function(input);
		for(int i = 0; i < N; i++)
			input_pos[i] = inv_lerp(input_pos[i], sourceStats.first[i], sourceStats.second[i]);
		for(auto& entry : pal)
		{
			float distance = 0.0f;
			auto result = analysis_function(entry);
			for(int i = 0; i < N; i++)
			{
				auto axis = inv_lerp(result[i], paletteStats.first[i], paletteStats.second[i]);
				auto diff = axis - input_pos[i];
				distance += diff * diff * weights[i];
			}
			if(distance < lowest_distance)
			{
				best_entry = &entry;
				lowest_distance = distance;
			}
		}
		return *best_entry;
	}

	template<typename EntryT, typename StatsFn, typename StatsT>
	EntryT map_color_relative_multisample(const ColorPalette<EntryT>& pal, EntryT input, StatsFn analysis_function, const std::pair<StatsT, StatsT>& paletteStats, const std::pair<StatsT, StatsT>& sourceStats, StatsT weights) 
	{
		assert(pal.size() > 0);
		constexpr auto N = std::tuple_size<StatsT>::value;
		using AxisType = typename StatsT::value_type;

		// Arbitary choice, how many colors to take into account.
		// In testing there was no longer any noticable difference between the results between 6 and 7 samples.
		// However additional samples have only a minimal performance impact and a higher number of samples may prevent artifacts with ill formed palettes.
		constexpr int NumSamples = 12;
		float  sample_distance[NumSamples];
		const  EntryT* samples[NumSamples] = {};
		StatsT      sample_pos[NumSamples];
		std::uninitialized_fill_n(sample_distance, NumSamples, HUGE_VALF);
		// sample_pos remains uninitialized

		constexpr auto inv_lerp = [](AxisType val, AxisType min, AxisType max)
		{
			return (val - min) / (max - min);
		};

		auto calc_color_stats = [&](const EntryT& v)
		{
			auto stats = analysis_function(v);
			for(int i = 0; i < N; i++)
				stats[i] = inv_lerp(stats[i], paletteStats.first[i], paletteStats.second[i]);
			return stats;
		};

		auto stat_distance = [&](StatsT& a, StatsT& b)
		{
			float result = 0.0f;
			for(int i = 0; i < N; i++)
			{
				auto diff = a[i] - b[i];
				result += diff * diff * weights[i];
			}
			return std::sqrt(result);
		};

		StatsT input_pos = analysis_function(input);
		for(int i = 0; i < N; i++)
			input_pos[i] = inv_lerp(input_pos[i], sourceStats.first[i], sourceStats.second[i]);
		for(auto& entry : pal)
		{
			auto axis = calc_color_stats(entry);
			float distance = stat_distance(axis, input_pos);
			for(int i = 0; i < NumSamples; i++)
			{
				if(distance < sample_distance[i])
				{
					for(int j = NumSamples-1; j > i; j--)
					{
						samples[j] = samples[j-1];
						sample_distance[j] = sample_distance[j-1];
						sample_pos[j] = sample_pos[j-1];
					}
					samples[i] = &entry;
					sample_distance[i] = distance;
					sample_pos[i] = axis;
					break;
				}
			}
		}
		if(sample_distance[0] < 0.00001f)
			return *samples[0];

		int last = (std::find(samples, samples+NumSamples, nullptr)-samples);
		EntryT result = *samples[last-1];
		for(int pair = last-2; pair >= 0; pair--)
		{
			StatsT result_stats = calc_color_stats(result);
			float a_dist = stat_distance(result_stats, input_pos);
			float b_dist = stat_distance(sample_pos[pair], input_pos);
			float whole = a_dist + b_dist;
			float u_coord = a_dist / whole;
			float v_coord = b_dist / whole;
			result = result * v_coord + *samples[pair] * u_coord;
		}
		return result;
	}
}