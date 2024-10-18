#include "merge_sort_cpu.hpp"
#include "merge_sort_gpu.hpp"
#include "utility.hpp"
#include <iostream>
#include <utility>

int main()
{
    const unsigned long size = 10000000;
    const auto range = std::make_pair(-100000, 100000);
    const unsigned int threads_count = 16;
    omp_set_num_threads(threads_count);

    std::cout << "CPU:\n";

    std::vector<int> v;
    Utility::generateRandomInput(v, size, range);
    std::vector<int> tmp(v);

    Utility::measureExecutionTime("CPU Recursive Merge Sort", CPU::RecursiveMergeSort::sort<int>, tmp);
    tmp.assign(v.begin(), v.end());

    Utility::measureExecutionTime("CPU Iterative Merge Sort", CPU::IterativeMergeSort::sort<int>, tmp);
    tmp.assign(v.begin(), v.end());

    Utility::measureExecutionTime("CPU Parallel Iterative Merge Sort", CPU::IterativeMergeSort::sortParallel<int>, tmp);
    tmp.assign(v.begin(), v.end());

    std::cout << "GPU:\n";
    Utility::measureExecutionTime("GPU Iterative Merge Sort", GPU::IterativeMergeSort::sort, tmp);
    tmp.assign(v.begin(), v.end());

    return 0;
}