cmake_minimum_required(VERSION 3.0)

add_library(all_cases STATIC all_cases.cc basic_ranges.cc)
target_link_libraries(all_cases PUBLIC utils)
