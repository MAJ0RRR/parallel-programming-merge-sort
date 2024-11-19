#ifndef TESTS_HPP
#define TESTS_HPP

#include "utility.hpp"
#include "merge_sort_cpu.hpp"

void runTests(std::vector<int>& v, std::vector<int>& input)
    {
        using ms = std::chrono::duration<double, std::milli>;
        const int runCount = 10;
        std::array<ms, runCount> executionTime;
        std::cout<< "Running CPU::MergeSort::mergeSortParallel " << runCount << " times\n";
        for(int i = 0; i < runCount; i++)
        {
            executionTime[i] = Utility::measureExecutionTime("CPU::MergeSort::mergeSortParallel", 
                                [&]() {
                                    CPU::MergeSort::sort(v, PARALLEL);
                                });

            Utility::validateSort(v);
            v.assign(input.begin(), input.end());
        }
        ms total = std::accumulate(executionTime.begin(), executionTime.end(), ms(0));
        ms mean = total / runCount;
        auto executionTimeComparator = [](const ms& a, const ms& b) { return a.count() < b.count(); };
        auto minIt = std::min_element(executionTime.begin(), executionTime.end(),
                                  executionTimeComparator);
        auto maxIt = std::max_element(executionTime.begin(), executionTime.end(),
                                  executionTimeComparator);
        ms min = *minIt;
        ms max = *maxIt;
        ms uncertainty = (max - min) / 2;

        std::cout << "Mean: " << mean.count() << " ms\n";
        std::cout << "Uncertainty: " << uncertainty.count() << " ms\n";
    }

    void runAll(std::vector<int>& v, std::vector<int>& input)
    {
        // Merge Sort iTERATIVE
        Utility::measureExecutionTime("CPU::IterativeMergeSort::sortParallel", 
        [&]() {
            CPU::MergeSort::sort(v, ITERATIVE);
        });
        Utility::validateSort(v);
        v.assign(input.begin(), input.end());


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
    }

#endif //TESTS_HPP