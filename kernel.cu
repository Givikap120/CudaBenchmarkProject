#include <iostream>
#include "BenchmarkManager.h"
#include "MatrixTransposer.h"

std::vector<MatrixTransposer> getTransposers()
{
	return {
	};
}

int main()
{
	BenchmarkManager benchmarkManager(1000, 1024, 1024);
	std::vector<MatrixTransposer> transposers = getTransposers();

	std::cout << "Raw transpotiion:\n";

	for (MatrixTransposer& transposer : transposers)
	{
		double time = benchmarkManager.runRaw(transposer);
		std::cout << transposer.getName() << ": " << time << " ms\n";
	}

	std::cout << "\nCopied transpotiion:\n";

	for (MatrixTransposer& transposer : transposers)
	{
		double time = benchmarkManager.runWithCopy(transposer);
		std::cout << transposer.getName() << ": " << time << " ms\n";
	}

    return 0;
}