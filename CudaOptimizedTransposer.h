#pragma once
#include "CudaTransposer.h"

class CudaOptimizedTransposer : public CudaTransposer
{
	size_t tileDim, blockRows;

public:
	CudaOptimizedTransposer(size_t threadsPerBlock = DEFAULT_THREADS_PER_BLOCK, size_t tileDim = 32) : 
		CudaTransposer("CUDA Optimized", threadsPerBlock), tileDim(tileDim), blockRows(threadsPerBlock / tileDim) {}

protected:
	void launchKernel() override;
};

