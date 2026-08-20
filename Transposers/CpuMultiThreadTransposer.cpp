#include "CpuMultiThreadTransposer.h"

CpuMultiThreadTransposer::CpuMultiThreadTransposer(int workerCount) : MatrixTransposer("CPU Multi Thread")
{
	for (int i = 0; i < workerCount; ++i)
	{
		workers.emplace_back(&CpuMultiThreadTransposer::workerMain, this, i);
	}
}

CpuMultiThreadTransposer::~CpuMultiThreadTransposer()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		disposing = true;
	}

	startNotify.notify_all();

	for (std::thread& worker : workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}
}

void CpuMultiThreadTransposer::transpose()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		++workIndex;
		finishedWorkers = 0;
	}

	startNotify.notify_all();

	std::unique_lock<std::mutex> lock(mutex);
	endNotify.wait(lock, [this] { return finishedWorkers == workers.size(); });
}

void CpuMultiThreadTransposer::workerMain(size_t workerIndex)
{
	int currWorkIndex = 0;

	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			startNotify.wait(lock, [this, &currWorkIndex] { return disposing || workIndex != currWorkIndex; });

			if (disposing)
				return;

			currWorkIndex = workIndex;
		}

		workerJobIteration(workerIndex);
	}
}

void CpuMultiThreadTransposer::workerJobIteration(size_t workerIndex)
{
	size_t rowsPerWorker = input.rows / workers.size();
	size_t remainingRows = input.rows % workers.size();

	size_t startRow = workerIndex * rowsPerWorker + std::min(workerIndex, remainingRows);
	size_t endRow = startRow + rowsPerWorker + (workerIndex < remainingRows ? 1 : 0);

	for (size_t r = startRow; r < endRow; ++r)
	{
		for (size_t c = 0; c < input.cols; ++c)
		{
			output(c, r) = input(r, c);
		}
	}

	std::lock_guard<std::mutex> lock(mutex);

	++finishedWorkers;

	if (finishedWorkers == workers.size())
	{
		endNotify.notify_one();
	}
}
