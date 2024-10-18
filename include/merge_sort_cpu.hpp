#ifndef MERGE_SORT_CPU_HPP
#define MERGE_SORT_CPU_HPP

#include <vector>
#include <omp.h>
#include <algorithm>

namespace CPU::RecursiveMergeSort
{
    template <typename T>
    void merge(std::vector<T>& data, unsigned long left, unsigned long mid, unsigned long right) {
        unsigned long leftSize = mid - left + 1;
        unsigned long rightSize = right - mid;

        std::vector<T> leftVec(leftSize);
        std::vector<T> rightVec(rightSize);

        unsigned long i,j,k;

        for (i = 0; i < leftSize; i++)
            leftVec[i] = data[left + i];

        for (i = 0; i < rightSize; i++)
            rightVec[i] = data[mid + 1 + i];

        i = 0; j = 0; k = left;

        while (i < leftSize && j < rightSize) {
            if (leftVec[i] <= rightVec[j]) {
                data[k] = leftVec[i];
                i++;
            } else {
                data[k] = rightVec[j];
                j++;
            }
            k++;
        }

        while (i < leftSize) {
            data[k] = leftVec[i];
            i++;
            k++;
        }

        while (j < rightSize) {
            data[k] = rightVec[j];
            j++;
            k++;
        }
    }

    template <typename T>
    void mergeSort(std::vector<T>& data, unsigned long left, unsigned long right) {
        if (left < right) {
            unsigned long mid = (left + right) /2;

            mergeSort(data, left, mid);
            mergeSort(data, mid + 1, right);

            merge(data, left, mid, right);
        }
    }

    template <typename T>
    void sort(std::vector<T>& data) {
        if (!data.empty()) {
            mergeSort(data, 0, data.size() - 1);
        }
    }
}

namespace CPU::IterativeMergeSort
{
    template <typename T> 
    void merge(std::vector<T>& data, std::vector<T>& tmp, unsigned long left, unsigned long mid, unsigned long right) {
        std::copy(data.begin() + left, data.begin() + right + 1, tmp.begin() + left);

        std::merge(tmp.begin() + left, tmp.begin() + mid + 1, tmp.begin() + mid + 1, tmp.begin() + right + 1, data.begin() + left);
    }

    template <typename T> 
    void sortParallel(std::vector<T>& data) {
        unsigned long size = data.size();
        std::vector<T> tmp(size);

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

    template <typename T> 
    void sort(std::vector<T>& data) {
        unsigned long size = data.size();
        std::vector<T> tmp(size);
        unsigned long left, mid, right;

        for(unsigned long current_size = 1; current_size <= size - 1; current_size *= 2)
        {
                for(left = 0; left < size - 1; left += 2 * current_size)
                {
                    mid = std::min(left + current_size - 1, size - 1);
                    right = std::min(left + 2 * current_size - 1, size - 1);

                    merge(data, tmp, left, mid, right);
                }
        }
    }
}

#endif // MERGE_SORT_CPU_HPP