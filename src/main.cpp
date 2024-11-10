#include "merge_sort_cpu.hpp"
#include "utility.hpp"
#include <iostream>
#include <utility>
#include <algorithm>

int main()
{
    // Problem size
    const unsigned long size = 10000000;
    const auto range = std::make_pair(-100000, 100000);

    // OpenMP Setup
    const unsigned int threads_count = omp_get_num_procs();
    omp_set_num_threads(threads_count);
    std::cout<< "Starting with " << threads_count << " threads.\n";

    // App Setup
    std::vector<int> input;
    std::cout<< "Generating random input vector of size " << size << " from values (" << range.first << ";" << range.second << ")...\n";
    Utility::measureExecutionTime("generateRandomInput", 
        [&]() {
                Utility::generateRandomInput<int>(input, size, range);
        });
    std::vector<int> v(input);

    // STL Sort
    Utility::measureExecutionTime("std::sort", 
     [&]() {
        std::sort(v.begin(), v.end());
    });
    Utility::validateSort(v);
    v.assign(input.begin(), input.end());

    // Merge Sort Sequential
    Utility::measureExecutionTime("CPU::MergeSort::mergeSortSequential", 
     [&]() {
        CPU::MergeSort::sort(v, SEQUENTIAL);
    });
    Utility::validateSort(v);
    v.assign(input.begin(), input.end());

    // Merge Sort Sequential
    Utility::measureExecutionTime("CPU::MergeSort::mergeSortSequentialWithCutOff", 
     [&]() {
        CPU::MergeSort::sort(v, SEQUENTIAL_WITH_CUT_OFF);
    });
    Utility::validateSort(v);
    v.assign(input.begin(), input.end());

    // Merge Sort Parallel
    Utility::measureExecutionTime("CPU::MergeSort::mergeSortParallel", 
     [&]() {
        CPU::MergeSort::sort(v, PARALLEL);
    });
    Utility::validateSort(v);
    v.assign(input.begin(), input.end());

    return 0;
}