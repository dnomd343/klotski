cmake_minimum_required(VERSION 3.0)

include_directories(../klotski/all_cases)

add_library(benchmark chore.cc benchmark.cc)
target_link_libraries(benchmark klotski)
