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
		constexpr auto NumAxis = std::tuple_size_v<StatsT>;
		using AxisType = typename StatsT::value_type;

		float lowest_distance = HUGE_VALF;
		const EntryT* best_entry = &input;
		auto inv_lerp = [](AxisType val, AxisType min, AxisType max)
		{
			return (val - min) / (max - min);
		};

		StatsT normalized_input_stats = analysis_function(input);
		for(int i = 0; i < NumAxis; i++)
			normalized_input_stats[i] = inv_lerp(normalized_input_stats[i], sourceStats.first[i], sourceStats.second[i]);
		for(auto& entry : pal)
		{
			float distance = 0.0f;
			auto result = analysis_function(entry);
			for(int i = 0; i < NumAxis; i++)
			{
				auto axis = inv_lerp(result[i], paletteStats.first[i], paletteStats.second[i]);
				auto diff = axis - normalized_input_stats[i];
				distance += (diff * diff) * weights[i];
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
		constexpr auto NumAxis = std::tuple_size<StatsT>::value;
		using AxisType = typename StatsT::value_type;

		// We need as many samples as vertices of a hypercube in that dimension
		// 1 axis = line segment (2 vertices/samples), 2 axis = quad (4 samples)
		// 3 axis = hexahedron (8 samples), 4 axis = octachoron (16 samples), etc
		constexpr int NumSamples = std::pow(2, NumAxis);
		float sample_distance[NumSamples];
		const EntryT* samples[NumSamples];
		std::uninitialized_fill_n(sample_distance, NumSamples, HUGE_VALF);
		std::uninitialized_fill_n(samples, NumSamples, nullptr);

		constexpr auto inv_lerp = [](AxisType val, AxisType min, AxisType max)
		{
			return (val - min) / (max - min);
		};

		StatsT normalized_input_stats = analysis_function(input);
		for(int i = 0; i < NumAxis; i++)
			normalized_input_stats[i] = inv_lerp(normalized_input_stats[i], sourceStats.first[i], sourceStats.second[i]);
		for(auto& entry : pal)
		{
			float distance = 0.0f;
			auto axis = analysis_function(entry);
			for(int i = 0; i < NumAxis; i++)
			{
				auto value = inv_lerp(axis[i], paletteStats.first[i], paletteStats.second[i]);
				auto diff = (value - normalized_input_stats[i]) * weights[i];
				distance += diff * diff;
			}
			for(int i = 0; i < NumSamples; i++)
			{
				if(distance < sample_distance[i])
				{
					for(int j = NumSamples-1; j > i; j--)
					{
						samples[j] = samples[j-1];
						sample_distance[j] = sample_distance[j-1];
					}
					samples[i] = &entry;
					sample_distance[i] = distance;
					break;
				}
			}
		}
		if(sample_distance[0] < 0.001f)
			return *samples[0];

		EntryT result = EntryT(0.0f);
		float accum = 0.0;
		constexpr float bias = 0.0001;
		for(int i = 0; i < NumSamples; i++)
		{
			if(samples[i] != nullptr)
			{
				result += *samples[i] / (sample_distance[i] + bias);
				accum += 1.0 / (sample_distance[i] + bias);
			}
		}
		result /= accum;
		return result;
	}
}