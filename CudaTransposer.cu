#include "CudaTransposer.h"
#include <stdexcept>

CudaTransposer::CudaTransposer(const std::string& name, size_t threadsPerBlock) : MatrixTransposer(name), threadsPerBlock(threadsPerBlock)
{
	try 
	{
// 		cudaCheck(cudaEventCreate(&startEvent));
// 		cudaCheck(cudaEventCreate(&stopEvent));
		cudaCheck(cudaStreamCreate(&stream));
	}
	catch (...)
	{
		dispose();
		throw;
	}
}

CudaTransposer::~CudaTransposer()
{
	dispose();
}

void CudaTransposer::copyInput(const Matrix& matrix)
{
	size_t size = matrix.rows * matrix.cols * sizeof(float);

	if (size != allocatedSize)
	{
		if (d_input)
			cudaCheck(cudaFree(d_input));

		if (d_output)
			cudaCheck(cudaFree(d_output));

		cudaCheck(cudaMalloc(&d_input, size));
		cudaCheck(cudaMalloc(&d_output, size));

		allocatedSize = size;
	}

	cudaCheck(cudaMemcpyAsync(d_input, matrix.data.data(), size, cudaMemcpyHostToDevice, stream));

	output = Matrix(matrix.cols, matrix.rows);
}

void CudaTransposer::transpose()
{
	size_t matrixSize = output.data.size() * sizeof(float);

// 	cudaCheck(cudaEventRecord(startEvent, stream));
	launchKernel();
	cudaCheck(cudaGetLastError());
// 	cudaCheck(cudaEventRecord(stopEvent, stream));
// 	cudaCheck(cudaEventSynchronize(stopEvent));
// 	cudaCheck(cudaEventElapsedTime(&kernelTime, startEvent, stopEvent));
	cudaCheck(cudaMemcpyAsync(output.data.data(), d_output, matrixSize, cudaMemcpyDeviceToHost, stream));
}

void CudaTransposer::synchronize()
{
	cudaCheck(cudaStreamSynchronize(stream));
}

void CudaTransposer::cudaCheck(cudaError_t result)
{
	if (result == cudaSuccess)
		return;

	throw std::runtime_error(cudaGetErrorString(result));
}

void CudaTransposer::dispose()
{
// 	if (startEvent) cudaEventDestroy(startEvent);
// 	if (stopEvent) cudaEventDestroy(stopEvent);
	if (stream) cudaStreamDestroy(stream);
	if (d_input) cudaFree(d_input);
	if (d_output) cudaFree(d_output);
}
