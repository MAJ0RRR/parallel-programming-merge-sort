#ifndef MERGE_SORT_CPU_HPP
#define MERGE_SORT_CPU_HPP

#include <vector>
#include <omp.h>
#include <algorithm>

enum OptimatizationLevel
{
    SEQUENTIAL = 0,
    SEQUENTIAL_WITH_CUT_OFF = 1,
    PARALLEL = 2,
    ITERATIVE = 3
};

namespace CPU::IterativeMergeSort
{
    void merge(std::vector<int>& data, std::vector<int>& tmp, unsigned long left, unsigned long mid, unsigned long right) {
        std::copy(data.begin() + left, data.begin() + right + 1, tmp.begin() + left);

        std::merge(tmp.begin() + left, tmp.begin() + mid + 1, tmp.begin() + mid + 1, tmp.begin() + right + 1, data.begin() + left);
    }

    void sortParallel(std::vector<int>& data) {
        unsigned long size = data.size();
        std::vector<int> tmp(size);

        for(unsigned long current_size = 1; current_size <= size - 1; current_size *= 2)
        {
            #pragma omp parallel for
                for(unsigned long left = 0; left < size - 1; left += 2 * current_size)
                {
                    unsigned long mid = std::min(left + current_size - 1, size - 1);
                    unsigned long right = std::min(left + 2 * current_size - 1, size - 1);

                    merge(data, tmp, left, mid, right);
                }
        }
    }
}

namespace CPU::MergeSort {
  void mergeSortSequential(std::vector<int>& data, unsigned long left, unsigned long right) {
        if (left < right) {
            unsigned long mid = (left + right) /2;

            mergeSortSequential(data, left, mid);
            mergeSortSequential(data, mid + 1, right);

            inplace_merge(data.begin()+left, data.begin()+mid+1, data.begin()+right+1);        
        }
    }

    void mergeSortSequentialWithCutOff(std::vector<int>& data, unsigned long left, unsigned long right) {
        if (left < right) {
            if(right - left >= 1024) {
                unsigned long mid = (left + right) /2;

                mergeSortSequentialWithCutOff(data, left, mid);
                mergeSortSequentialWithCutOff(data, mid + 1, right);
                inplace_merge(data.begin()+left, data.begin()+mid+1, data.begin()+right+1);        
            }
            else {
                sort(data.begin()+left, data.begin()+right+1);
            }
        }
    }

    void mergeSortParallel(std::vector<int>& data, unsigned long left, unsigned long right) {
        if (left < right) {
            if(right - left >= 1024) {
                unsigned long mid = (left + right) /2;
                #pragma omp taskgroup
                {
                    #pragma omp task shared(data)
                    mergeSortParallel(data, left, mid);
                    mergeSortParallel(data, mid + 1, right);
                }
                inplace_merge(data.begin()+left, data.begin()+mid+1, data.begin()+right+1);        
            }
            else {
                sort(data.begin()+left, data.begin()+right+1);
            }
        }
    }

    void sort(std::vector<int>& data, OptimatizationLevel Olevel) {
        if (!data.empty()) {
            switch (Olevel)
            {
                case SEQUENTIAL:
                    mergeSortSequential(data, 0, data.size() - 1);
                break;
                case SEQUENTIAL_WITH_CUT_OFF:
                    mergeSortSequentialWithCutOff(data, 0, data.size() - 1);
                break;
                case PARALLEL:
                    #pragma omp parallel
                    #pragma omp single
                    mergeSortParallel(data, 0, data.size() - 1);
                break;
                case ITERATIVE:
                    CPU::IterativeMergeSort::sortParallel(data);
                break;
            }
        }
    }
}

#endif // MERGE_SORT_CPU_HPP