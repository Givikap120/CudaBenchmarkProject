#include "BenchmarkManager.h"

#include <random>

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

double BenchmarkManager::runRaw(MatrixTransposer& transposer)
{
	return 0;
}

double BenchmarkManager::runWithCopy(MatrixTransposer& transposer)
{
	return 0;
}
