#include <iostream>

#include "BenchmarkManager.h"
#include "MatrixTransposer.h"

#include "CpuSingleThreadTransposer.h"
#include "CpuMultiThreadTransposer.h"
#include "CudaNaiveTransposer.h"
#include "CudaOptimizedTransposer.h"

std::vector<std::unique_ptr<MatrixTransposer>> getTransposers()
{
	std::vector<std::unique_ptr<MatrixTransposer>> result;

	result.emplace_back(std::make_unique<CpuSingleThreadTransposer>());
	result.emplace_back(std::make_unique<CpuMultiThreadTransposer>(16));
	result.emplace_back(std::make_unique<CudaNaiveTransposer>());
	result.emplace_back(std::make_unique<CudaOptimizedTransposer>());

	return result;
}

int main()
{
	BenchmarkManager benchmarkManager(1000, 1024, 1024);
	std::vector<std::unique_ptr<MatrixTransposer>> transposers = getTransposers();

	// Warm up
	for (auto& transposer : transposers)
		benchmarkManager.runRaw(transposer.get(), false);

	std::cout << "Raw transposition:\n";

	for (auto& transposer : transposers)
		benchmarkManager.runRaw(transposer.get());

	std::cout << "\nCopied transposition:\n";

	for (auto& transposer : transposers)
		benchmarkManager.runWithCopy(transposer.get());

    return 0;
}