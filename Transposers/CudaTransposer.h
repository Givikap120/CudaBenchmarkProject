#pragma once
#include "MatrixTransposer.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

class CudaTransposer : public MatrixTransposer
{
 	float kernelTime = 0.0f;
 	cudaEvent_t startEvent{}, stopEvent{};

protected:
	size_t threadsPerBlock;
	size_t allocatedSize = 0;
		
	float* d_input = nullptr;
	float* d_output = nullptr;
	cudaStream_t stream{};

public:
	static constexpr size_t DEFAULT_THREADS_PER_BLOCK = 256;

	CudaTransposer(const std::string& name, size_t threadsPerBlock);
	~CudaTransposer() override;

	void copyInput(const Matrix& matrix) override;
	void transpose() override;

	void synchronize() override;
	
	float getKernelTime() const { return kernelTime; }

protected:
	virtual void launchKernel() = 0;

private:
	void cudaCheck(cudaError_t result);
	void dispose();
};
