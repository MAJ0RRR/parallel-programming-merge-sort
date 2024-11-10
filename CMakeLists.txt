cmake_minimum_required(VERSION 3.18)

project(MERGE_SORT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenMP REQUIRED)

set(SOURCES
    src/main.cpp
)

add_executable(main ${SOURCES})

if(OpenMP_CXX_FOUND)
    target_link_libraries(main OpenMP::OpenMP_CXX)
endif()
