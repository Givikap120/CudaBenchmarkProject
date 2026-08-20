#include "BenchmarkManager.h"
#include "MatrixTransposer.h"

#include <random>
#include <chrono>

BenchmarkManager::BenchmarkManager(int iterations, size_t rows, size_t cols) : iterations(iterations), matrix(generateMatrix(rows, cols))
{}

Matrix BenchmarkManager::generateMatrix(size_t rows, size_t cols)
{
	Matrix matrix(rows, cols);
	std::mt19937 gen(727);
	std::uniform_real_distribution<float> dis(-1000.0f, 1000.0f);
	
	for (float& val : matrix.data) {
		val = dis(gen);
	}

	return matrix;
}

static double getTime()
{
	auto time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double, std::milli>(time.time_since_epoch()).count();
}

double BenchmarkManager::runRaw(MatrixTransposer& transposer)
{
	transposer.copyInput(matrix);
	transposer.synchronize();

	double time = getTime();

	for (int i = 0; i < iterations; ++i)
	{
		transposer.transpose();
		transposer.synchronize();
	}

	return (getTime() - time) / iterations;
}

double BenchmarkManager::runWithCopy(MatrixTransposer& transposer)
{
	double time = getTime();

	for (int i = 0; i < iterations; ++i)
	{
		transposer.copyInput(matrix);
		transposer.transpose();
		transposer.synchronize();
	}

	return (getTime() - time) / iterations;
}
