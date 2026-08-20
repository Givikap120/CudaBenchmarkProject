#include "BenchmarkManager.h"
#include "Transposers/MatrixTransposer.h"
#include "Transposers/CudaTransposer.h"

#include <random>
#include <chrono>
#include <iostream>

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

static void printTransposerInfo(const MatrixTransposer* transposer, double executionTime, double kernelTime)
{
	std::cout << transposer->getName() << ": " << executionTime << " ms";

	if (kernelTime >= 0)
		std::cout << " (kernel time: " << kernelTime << " ms)";

	std::cout << '\n';
}

void BenchmarkManager::runRaw(MatrixTransposer* transposer, bool log)
{
	auto setupFunc = [this](MatrixTransposer* transposer)
		{
			transposer->copyInput(matrix);
			transposer->synchronize();
		};

	auto runFunc = [](MatrixTransposer* transposer)
		{
			transposer->transpose();
			transposer->synchronize();
		};

	runInternal(transposer, setupFunc, runFunc, log);
}

void BenchmarkManager::runWithCopy(MatrixTransposer* transposer, bool log)
{
	auto setupFunc = [](MatrixTransposer* transposer) {};

	auto runFunc = [this](MatrixTransposer* transposer)
		{
			transposer->copyInput(matrix);
			transposer->transpose();
			transposer->synchronize();
		};

	runInternal(transposer, setupFunc, runFunc, log);
}

void BenchmarkManager::runInternal(MatrixTransposer* transposer, std::function<void(MatrixTransposer*)> setupFunc, std::function<void(MatrixTransposer*)> runFunc, bool log)
{
	auto cudaTransposer = dynamic_cast<CudaTransposer*>(transposer);

	setupFunc(transposer);

	double startTime = getTime();
	double kernelTimeCumulative = 0.0;

	for (int i = 0; i < iterations; ++i)
	{
		runFunc(transposer);

		if (cudaTransposer)
			kernelTimeCumulative += cudaTransposer->getKernelTime();
	}

	double executionTime = (getTime() - startTime) / iterations;
	double kernelTime = cudaTransposer ? kernelTimeCumulative / iterations : -1;

	if (log) printTransposerInfo(transposer, executionTime, kernelTime);
}
