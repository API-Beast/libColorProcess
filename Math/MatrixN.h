#pragma once
#include "VectorN.h"

template<int InputN, int OutputN, typename T>
struct Matrix
{
	VectorN<OutputN, T> elements[InputN];
	VectorN<OutputN, T> transform(VectorN<InputN, T> input);
	Matrix<InputN, OutputN, T> multiply(const Matrix<InputN, OutputN, T>& other);
	Matrix<InputN, OutputN, T> invert();
	T determinant();
	int rank();

	static constexpr Matrix<InputN, OutputN, T> identity()
	{
		Matrix<InputN, OutputN, T> result;
		for(int i = 0; i<InputN; i++)
		{
			result.elements[i] = VectorN<OutputN, T>(0);
			if(i < OutputN) result.elements[i][i] = 1;
		}
		return result;
	}

	static constexpr Matrix<InputN, OutputN, T> scale(VectorN<InputN, T> scalar)
	{
		Matrix<InputN, OutputN, T> result;
		for(int i = 0; i<InputN; i++)
		{
			result.elements[i] = VectorN<OutputN, T>(0);
			if(i < OutputN) result.elements[i][i] = scalar[i];
		}
		return result;
	}
}