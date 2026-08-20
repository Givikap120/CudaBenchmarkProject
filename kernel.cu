
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <cstdlib>
#include <iostream>

void cudaCheck(cudaError_t call)
{                                
    if (call != cudaSuccess)
    {                                                         
        std::cerr << "CUDA error: "                           
                  << cudaGetErrorString(call)
                  << '\n';                                    
        std::exit(1);                                         
    }                                                         
}

__global__ void helloKernel()
{
    printf(
        "Hello from GPU thread %d\n",
        threadIdx.x
    );
}


int main()
{
    cudaDeviceProp properties{};

    cudaCheck(cudaGetDeviceProperties(&properties, 0));

    std::cout
        << "GPU: "
        << properties.name
        << '\n';

    std::cout
        << "Compute capability: "
        << properties.major
        << '.'
        << properties.minor
        << '\n';

    std::cout
        << "SM count: "
        << properties.multiProcessorCount
        << "\n\n";

    helloKernel<<<1, 4>>>();

    cudaCheck(cudaGetLastError());
    cudaCheck(cudaDeviceSynchronize());

    return 0;
}