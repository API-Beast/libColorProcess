#pragma once
#include <vector>
#include "VectorN.h"

template<int N, typename ValueT = void>
struct Simplex
{
	VectorN<N, float> points[N+1];
	ValueT            values[N+1];
	VectorN<N+1, float> convert_to_barycentric(VectorN<N, float> coord);
	ValueT interpolate_cardinal   (VectorN<N,   float> cardinal);
	ValueT interpolate_barycentric(VectorN<N+1, float> barycentric);
};

template<int N, typename ValueT>
struct SimplexHull
{
	std::vector<VectorN<N, float>> points;
	std::vector<Simplex<N, ValueT>> hull;
};