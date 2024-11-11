#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <random>
#include <utility>
#include <chrono>
#include <functional>
#include <iostream>
#include <array>
#include <fstream>

namespace Utility{
    template <typename T>
    void generateRandomInput(std::vector<T>& data, unsigned int size, std::pair<T, T> range) {
        data.resize(size);
        std::random_device rd;
        std::mt19937 generator(rd());

        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> distribution(range.first, range.second);
            for (unsigned int i = 0; i < size; ++i) {
                data[i] = distribution(generator);
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> distribution(range.first, range.second);
            for (unsigned int i = 0; i < size; ++i) {
                data[i] = distribution(generator);
            }
        }
    }

    template <typename Func, typename... Args>
    std::chrono::duration<double, std::milli> measureExecutionTime(const std::string& funcName, Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();

        std::invoke(std::forward<Func>(func));

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Execution time of " << funcName << ": " << duration.count() << " ms" << std::endl;
        return duration;
    }

    template <typename T>
    void validateSort(std::vector<T>& data)
    {
        bool isSorted = std::is_sorted(data.begin(), data.end());
        if (isSorted) {
            std::cout << "Vector is sorted correctly\n";
        }
        else {
            std::cout << "Vector is not sorted correctly\n";
        }
    }
}

#endif // UTILITY_HPP