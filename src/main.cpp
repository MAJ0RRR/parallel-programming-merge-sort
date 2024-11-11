#include "tests.hpp"
#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);
     if (args.empty()) {
        std::cerr << "No arguments provided. Try [all/test].\n";
        return 1;
    }

    // Problem parameters
    const unsigned long size = 100000000;
    const auto range = std::make_pair(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    // OpenMP Setup
    const unsigned int threads_count = omp_get_num_procs();
    omp_set_num_threads(threads_count);
    std::cout<< "Running with " << threads_count << " threads.\n";

    // App Setup
    std::vector<int> input;
    std::cout<< "Generating random input vector of size " << size << " from values (" << range.first << ";" << range.second << ")...\n";
    Utility::measureExecutionTime("generateRandomInput", 
        [&]() {
                Utility::generateRandomInput<int>(input, size, range);
        });
    std::vector<int> v(input);

    for (const auto& arg : args) {
        std::cout << "Mode: " << arg << std::endl;

        if(arg == "all") {
            runAll(v, input);
        }
        else if(arg == "test") {
            runTests(v, input);
        }
        else{
            std::cerr << "Invalid arguments. Try [all/test].\n";
            return 1;
        }
    }    
    return 0;
}