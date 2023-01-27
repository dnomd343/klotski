cmake_minimum_required(VERSION 3.0)

add_library(benchmark OBJECT chore.cc benchmark.cc)
#target_link_libraries(benchmark PUBLIC all_cases)
