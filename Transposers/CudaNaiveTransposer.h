#pragma once
#include "CudaTransposer.h"

class CudaNaiveTransposer : public CudaTransposer
{
public:
	CudaNaiveTransposer(size_t threadsPerBlock = DEFAULT_THREADS_PER_BLOCK) : CudaTransposer("CUDA Naive", threadsPerBlock) {}

protected:
	void launchKernel() override;
};

