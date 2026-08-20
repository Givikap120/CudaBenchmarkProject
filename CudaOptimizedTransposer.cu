#include "CudaOptimizedTransposer.h"

static __global__ void transposeKernel(const float* input, float* output, size_t rows, size_t cols, size_t tileDim, size_t blockRows)
{
	extern __shared__ float tile[];

	size_t tilePitch = tileDim + 1;

	size_t xIndex = blockIdx.x * tileDim + threadIdx.x;
	size_t yIndex = blockIdx.y * tileDim + threadIdx.y;

	for (size_t i = 0; i < tileDim; i += blockRows)
	{
		size_t tileY = threadIdx.y + i;

		if (tileY < tileDim && xIndex < cols && yIndex + i < rows)
			tile[tileY * tilePitch + threadIdx.x] = input[(yIndex + i) * cols + xIndex];
	}

	__syncthreads();

	xIndex = blockIdx.y * tileDim + threadIdx.x;
	yIndex = blockIdx.x * tileDim + threadIdx.y;

	for (size_t i = 0; i < tileDim; i += blockRows)
	{
		size_t tileY = threadIdx.y + i;

		if (tileY < tileDim && xIndex < rows && yIndex + i < cols)
			output[(yIndex + i) * rows + xIndex] = tile[threadIdx.x * tilePitch + tileY];
	}
}

void CudaOptimizedTransposer::launchKernel()
{
	size_t rows = output.cols;
	size_t cols = output.rows;

	dim3 blockSize(tileDim, blockRows);
	dim3 gridSize((cols + tileDim - 1) / tileDim, (rows + tileDim - 1) / tileDim);

	size_t sharedMemorySize = tileDim * (tileDim + 1) * sizeof(float);

	transposeKernel <<<gridSize, blockSize, sharedMemorySize, stream>>> (d_input, d_output, rows, cols, tileDim, blockRows);
}
