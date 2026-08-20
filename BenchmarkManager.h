#pragma once

#include "Matrix.h"
#include <functional>

class MatrixTransposer;

class BenchmarkManager
{
	int iterations;
	Matrix matrix;

public:
	BenchmarkManager(int iterations, size_t rows, size_t cols);
	static Matrix generateMatrix(size_t rows, size_t cols);

	int getIterations() const { return iterations; }

	void runRaw(MatrixTransposer* transposer, bool log = true);
	void runWithCopy(MatrixTransposer* transposer, bool log = true);

private:
	void runInternal(MatrixTransposer* transposer, std::function<void(MatrixTransposer*)> setupFunc, std::function<void(MatrixTransposer*)> runFunc, bool log);
};

