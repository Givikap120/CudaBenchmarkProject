#pragma once

#include "Matrix.h"

class MatrixTransposer;

class BenchmarkManager
{
	int iterations;
	Matrix matrix;

public:
	BenchmarkManager(int iterations, size_t rows, size_t cols);
	static Matrix generateMatrix(size_t rows, size_t cols);

	int getIterations() const { return iterations; }

	double runRaw(MatrixTransposer& transposer);
	double runWithCopy(MatrixTransposer& transposer);
};

