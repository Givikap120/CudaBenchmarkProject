#include <iostream>
#include "BenchmarkManager.h"
#include "MatrixTransposer.h"

#include "CpuSingleThreadTransposer.h"
#include "CpuMultiThreadTransposer.h"
#include "CudaNaiveTransposer.h"

std::vector<std::unique_ptr<MatrixTransposer>> getTransposers()
{
	std::vector<std::unique_ptr<MatrixTransposer>> result;

	result.emplace_back(std::make_unique<CpuSingleThreadTransposer>());
	result.emplace_back(std::make_unique<CpuMultiThreadTransposer>(16));
	result.emplace_back(std::make_unique<CudaNaiveTransposer>());

	return result;
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
		std::cout << transposer->getName() << ": " << time << " ms\n";
	}

	std::cout << "\nCopied transposition:\n";

	for (auto& transposer : transposers)
	{
		double time = benchmarkManager.runWithCopy(*transposer);
		std::cout << transposer->getName() << ": " << time << " ms\n";
	}

    return 0;
}