#include "merge_sort_gpu.hpp"
#include <cuda_runtime.h>
#include <iostream>

namespace GPU::IterativeMergeSort
{
    __device__ void merge(int* data, int* tmp, unsigned long left, unsigned long mid, unsigned long right) {
        unsigned long i, j, k;

        i = left; j = mid + 1; k = left; 

        while (i <= mid && j <= right) {
            if (data[i] <= data[j]) {
                tmp[k++] = data[i++];
            } else {
                tmp[k++] = data[j++];
            }
        }

        while (i <= mid) {
            tmp[k++] = data[i++];
        }
        
        while (j <= right) {
            tmp[k++] = data[j++];
        }

        for (i = left; i <= right; i++) {
            data[i] = tmp[i];
        }
    }

    __global__ void kernel(int* data, int* tmp, unsigned long size)
    {
        unsigned long left, mid, right;
        unsigned long idx = blockIdx.x * blockDim.x + threadIdx.x;
        for (unsigned long current_size = 1; current_size < size; size *= 2) {
            left = idx * 2 * current_size;
            mid = min(left + current_size - 1, size - 1);
            right = min(left + 2 * current_size - 1, size - 1);
        
            if (left < size) {
                merge(data, tmp, left, mid, right);
            }
        __syncthreads(); 
        }
    }

    void sort(std::vector<int>& data)
    {
        unsigned long size = data.size();
        int* dData;
        int* dTmp;

        cudaMalloc((void**)&dData, size * sizeof(int));
        cudaMalloc((void**)&dTmp, size * sizeof(int));

        cudaMemcpy(dData, data.data(), size * sizeof(int), cudaMemcpyHostToDevice);

        cudaEvent_t start, stop;
            float elapsedTime;

        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        cudaEventRecord(start);

        kernel<<<128,1024>>>(dData, dTmp, size);

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);

        cudaEventElapsedTime(&elapsedTime, start, stop);

        std::cout << "Kernel execution time: " << elapsedTime << "ms\n";

        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        cudaMemcpy(data.data(), dData, size * sizeof(int), cudaMemcpyDeviceToHost);

        cudaFree(dData);
        cudaFree(dTmp);
    }
}