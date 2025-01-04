#ifndef TESTS_HPP
#define TESTS_HPP

#include "utility.hpp"
#include "merge_sort_gpu.hpp"

void runTests(std::vector<int>& v, std::vector<int>& input)
    {
        using ms = std::chrono::duration<double, std::milli>;
        const int runCount = 1;
        std::array<ms, runCount> executionTime;
        std::cout<< "Running GPU::MergeSort::sort " << runCount << " times\n";
        for(int i = 0; i < runCount; i++)
        {
            executionTime[i] = Utility::measureExecutionTime("GPU::MergeSort::sort", 
                                [&]() {
                                    GPU::MergeSort::sort(v);
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

#endif //TESTS_HPP