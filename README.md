# CUDA Benchmark Project

A small C++/CUDA benchmark comparing different approaches to matrix transposition on the CPU and GPU.

The project implements the same matrix transpose using:

- Single-threaded CPU execution
- Multi-threaded CPU execution
- A simple CUDA kernel
- An optimized CUDA kernel using tiled shared memory

It benchmarks both the raw transpose operation and the complete operation including input copying.

## Implementations

### CPU Single Thread

A straightforward single-threaded matrix transpose used as the baseline implementation.

Each matrix element is copied to its transposed position:

```cpp
output(c, r) = input(r, c);
```

### CPU Multi Thread

Uses a persistent pool of `std::thread` workers.

The matrix rows are divided between the workers, with each worker processing a separate contiguous range. The worker threads are created once and reused between benchmark iterations to avoid measuring repeated thread creation/destruction overhead.

The default benchmark uses **16 worker threads**.

### CUDA Naive

A basic CUDA implementation where each CUDA thread processes one matrix element.

The matrix is flattened into a one-dimensional index:

```cpp
size_t i = blockIdx.x * blockDim.x + threadIdx.x;
```

and each thread calculates the corresponding row and column before writing the transposed element.

This version provides a simple GPU baseline without using shared-memory optimizations.

### CUDA Optimized

An optimized tiled transpose using CUDA shared memory.

The matrix is split into tiles. Each CUDA block:

1. Reads a tile from global GPU memory using coalesced accesses.
2. Stores it in shared memory.
3. Synchronizes the threads in the block.
4. Reads the shared-memory tile in transposed order.
5. Writes the result back to global memory using coalesced accesses.

The shared-memory tile has an additional column of padding:

```text
tileDim × (tileDim + 1)
```

to reduce shared-memory bank conflicts during the transposed access.

With the default configuration:

```text
tileDim        = 32
threads/block  = 256
block size     = 32 × 8
```

Each block processes a logical `32 × 32` tile using 256 CUDA threads.

## Benchmark Modes

The benchmark runs every implementation multiple times and reports the average execution time.

### Raw transposition

The input matrix is copied into the transposer before timing begins.

The benchmark then repeatedly performs only the transpose operation:

```text
copy input once

transpose
transpose
transpose
...
```

For CUDA, the wall-clock time includes producing the result on the GPU and transferring the result back to host memory.

### Copied transposition

The input is copied before every transpose:

```text
copy input
transpose

copy input
transpose

...
```

For CUDA this includes the host-to-device input transfer in addition to the GPU operation and device-to-host result transfer.

This makes it possible to see how much GPU transfer overhead affects the practical performance advantage of CUDA.

## CUDA Timing

CUDA implementations additionally use CUDA events to measure CUDA-side execution time.

This helps distinguish the execution cost of GPU work from the complete CPU-observed operation time.

Because CUDA operations are asynchronous, the benchmark explicitly synchronizes before considering an operation complete.

## Example Results

Example output for a `1024 × 1024` matrix over 1000 iterations:

```text
Raw transposition:
CPU Single Thread: 7.0649 ms
CPU Multi Thread: 0.727489 ms
CUDA Naive: 0.714942 ms (kernel time: 0.1897 ms)
CUDA Optimized: 0.453573 ms (kernel time: 0.049509 ms)

Copied transposition:
CPU Single Thread: 7.89442 ms
CPU Multi Thread: 1.54249 ms
CUDA Naive: 1.93786 ms (kernel time: 0.169382 ms)
CUDA Optimized: 1.69723 ms (kernel time: 0.0585824 ms)
```

Results depend heavily on CPU model, GPU model, memory speed, CUDA version, matrix size, thread count, and build configuration.

For meaningful performance measurements, build and run the project in **Release x64** mode.

## Requirements

- Windows
- NVIDIA CUDA-capable GPU
- NVIDIA CUDA Toolkit
- Visual Studio 2022 with the Desktop development with C++ workload
- x64 build target

The included Visual Studio project currently uses:

```text
Platform Toolset: v143
CUDA Toolkit:     12.8
```

If a different CUDA Toolkit version is installed, update the CUDA Build Customization used by the Visual Studio project.

## Building

Clone the repository:

```bash
git clone https://github.com/Givikap120/CudaBenchmarkProject.git
cd CudaBenchmarkProject
```

Open:

```text
CudaTestProject.sln
```

in Visual Studio.

Select:

```text
Release
x64
```

and build the solution.

You can build from Visual Studio using:

```text
Build → Build Solution
```

or:

```text
Ctrl+Shift+B
```
