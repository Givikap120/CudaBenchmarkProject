#include "CudaNaiveTransposer.h"

static __global__ void transposeKernel(const float* input, float* output, size_t rows, size_t cols)
{
	size_t i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i >= rows * cols)
		return;

	size_t row = i / cols;
	size_t col = i % cols;

	output[col * rows + row] = input[i];
}

void CudaNaiveTransposer::launchKernel()
{
	size_t blockCount = (output.data.size() + threadsPerBlock - 1) / threadsPerBlock;

	transposeKernel <<<blockCount, threadsPerBlock, 0, stream>>> (d_input, d_output, output.rows, output.cols);
}
