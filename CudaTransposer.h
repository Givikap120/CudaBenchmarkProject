#pragma once
#include "MatrixTransposer.h"
#include <cuda_runtime.h>

class CudaTransposer : public MatrixTransposer
{
	float kernelTime = 0.0f;
	cudaEvent_t startEvent{}, stopEvent{};

protected:
	float* d_input = nullptr;
	float* d_output = nullptr;
	cudaStream_t stream{};

public:
	CudaTransposer(const std::string& name);
	~CudaTransposer() override;

	void copyInput(const Matrix& matrix) override;
	void transpose() override;
	double getTime() override { return kernelTime; }

protected:
	virtual void launchKernel() = 0;

private:
	void cudaCheck(cudaError_t result);
	void dispose();
};
