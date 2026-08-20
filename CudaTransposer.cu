#include "CudaTransposer.h"
#include <stdexcept>

CudaTransposer::CudaTransposer(const std::string& name) : MatrixTransposer(name)
{
	try 
	{
		cudaCheck(cudaEventCreate(&startEvent));
		cudaCheck(cudaEventCreate(&stopEvent));
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
	size_t matrixSize = matrix.rows * matrix.cols * sizeof(float);

	if (d_input) cudaFree(d_input);
	if (d_output) cudaFree(d_output);

	cudaCheck(cudaMalloc(&d_input, matrixSize));
	cudaCheck(cudaMalloc(&d_output, matrixSize));
	cudaCheck(cudaMemcpyAsync(d_input, matrix.data.data(), matrixSize, cudaMemcpyHostToDevice, stream));

	output = Matrix(matrix.cols, matrix.rows);

	// The immediate synchronization is not needed
	// cudaCheck(cudaStreamSynchronize(stream));
}

void CudaTransposer::transpose()
{
	size_t matrixSize = output.data.size() * sizeof(float);

	// Calculate
	cudaCheck(cudaEventRecord(startEvent, stream));
	launchKernel();
	cudaCheck(cudaGetLastError());
	cudaCheck(cudaEventRecord(stopEvent, stream));
	cudaCheck(cudaEventSynchronize(stopEvent));
	cudaCheck(cudaEventElapsedTime(&kernelTime, startEvent, stopEvent));

	// Copy result back to host
	cudaCheck(cudaMemcpyAsync(output.data.data(), d_output, matrixSize, cudaMemcpyDeviceToHost, stream));
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
	if (startEvent) cudaEventDestroy(startEvent);
	if (stopEvent) cudaEventDestroy(stopEvent);
	if (stream) cudaStreamDestroy(stream);
	if (d_input) cudaFree(d_input);
	if (d_output) cudaFree(d_output);
}
