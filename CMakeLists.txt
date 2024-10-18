cmake_minimum_required(VERSION 3.18)

project(MERGE_SORT LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenMP REQUIRED)

find_package(CUDAToolkit REQUIRED)

set(CMAKE_CUDA_ARCHITECTURES 80)

set(SOURCES
    src/main.cpp
    src/merge_sort_gpu.cu
)

add_executable(main ${SOURCES})

if(OpenMP_CXX_FOUND)
    target_link_libraries(main OpenMP::OpenMP_CXX)
endif()