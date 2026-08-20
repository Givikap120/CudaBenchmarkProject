#pragma once

#include "MatrixTransposer.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class CpuMultiThreadTransposer : public MatrixTransposer
{
	std::vector<std::thread> workers;

	std::mutex mutex;
	std::condition_variable startNotify, endNotify;

	bool disposing = false;
	size_t workIndex = 0;
	int finishedWorkers = -1;

public:
	CpuMultiThreadTransposer(int workerCount);
	~CpuMultiThreadTransposer() override;

	void transpose() override;

private:
	void workerMain(size_t workerIndex);
	void workerJobIteration(size_t workerIndex);
};

