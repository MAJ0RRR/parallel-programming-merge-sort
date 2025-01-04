#include "merge_sort_gpu.hpp"
#include <cuda_runtime.h>
#include <iostream>
#include "utility.hpp"

namespace GPU::MergeSort
{
    __global__ void nonOptimizedKernel(int* dData, int* dTmp, unsigned long size, unsigned long chunkSize)
    {
        unsigned long idx = blockIdx.x * blockDim.x + threadIdx.x;
        unsigned long start1 = idx * 2 * chunkSize;
        unsigned long end1 = min(start1 + chunkSize, size);
        unsigned long start2 = end1;
        unsigned long end2 = min(start2 + chunkSize, size);

        unsigned long i = start1, j = start2, k = start1;

        // Merge two sorted chunks
        while (i < end1 && j < end2) {
            if (dData[i] <= dData[j]) {
                dTmp[k++] = dData[i++];
            } else {
                dTmp[k++] = dData[j++];
            }
        }

        while (i < end1) dTmp[k++] = dData[i++];
        while (j < end2) dTmp[k++] = dData[j++];
    }

    void sort(std::vector<int>& data)
    {
        unsigned long size = data.size();
        int* dData;
        int* dTmp;

        cudaError_t err;

        err = cudaMalloc((void**)&dData, size * sizeof(int));
        if (err != cudaSuccess) {
            std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
            return;
        }

        err = cudaMalloc((void**)&dTmp, size * sizeof(int));
        if (err != cudaSuccess) {
            std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
            cudaFree(dData);
            return;
        }

        err = cudaMemcpy(dData, data.data(), size * sizeof(int), cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
            cudaFree(dData);
            cudaFree(dTmp);
            return;
        }

        // Debugging: Copy data back to host to check if it was correctly copied
        std::vector<int> hostData(size);
        err = cudaMemcpy(hostData.data(), dData, size * sizeof(int), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
            cudaFree(dData);
            cudaFree(dTmp);
            return;
        }

        cudaEvent_t start, stop;
        float elapsedTime;

        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        cudaEventRecord(start);

        unsigned long chunkSize = 1;
        while (chunkSize < size) {
            unsigned long numChunks = (size + 2 * chunkSize - 1) / (2 * chunkSize);
            unsigned int threadsPerBlock = 256;
            unsigned int blocksPerGrid = (numChunks + threadsPerBlock - 1) / threadsPerBlock;

            // Launch kernel
            nonOptimizedKernel<<<blocksPerGrid, threadsPerBlock>>>(dData, dTmp, size, chunkSize);
            // optimizedKernel<<<blocksPerGrid, threadsPerBlock, sharedMemSize>>>(dData, dTmp, size, chunkSize);

            // Check for kernel launch errors
            err = cudaGetLastError();
            if (err != cudaSuccess) {
                std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
                cudaFree(dData);
                cudaFree(dTmp);
                return;
            }

            // Swap input and output pointers
            std::swap(dData, dTmp);

            chunkSize *= 2;
        }

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);

        cudaEventElapsedTime(&elapsedTime, start, stop);

        std::cout << "Kernel execution time: " << elapsedTime << "ms\n";

        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        err = cudaMemcpy(data.data(), dData, size * sizeof(int), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            std::cerr << "CUDA error: " << cudaGetErrorString(err) << std::endl;
        }

        cudaFree(dData);
        cudaFree(dTmp);
    }
}
