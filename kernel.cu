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

void printTransposerInfo(const MatrixTransposer* transposer, double executionTime)
{
	std::cout << transposer->getName() << ": " << executionTime << " ms";

	auto cudaTransposer = dynamic_cast<const CudaTransposer*>(transposer);
	if (cudaTransposer)
		std::cout << " (kernel time: " << cudaTransposer->getKernelTime() << " ms)";

	std::cout << '\n';
}

int main()
{
	BenchmarkManager benchmarkManager(100, 1024, 1024);
	std::vector<std::unique_ptr<MatrixTransposer>> transposers = getTransposers();

	// Warm up
	for (auto& transposer : transposers)
		benchmarkManager.runRaw(*transposer);

	std::cout << "Raw transposition:\n";

	for (auto& transposer : transposers)
	{
		double time = benchmarkManager.runRaw(*transposer);
		printTransposerInfo(transposer.get(), time);
	}

	std::cout << "\nCopied transposition:\n";

	for (auto& transposer : transposers)
	{
		double time = benchmarkManager.runWithCopy(*transposer);
		printTransposerInfo(transposer.get(), time);
	}

    return 0;
}