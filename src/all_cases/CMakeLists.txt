cmake_minimum_required(VERSION 3.0)

add_library(all_cases basic_ranges.cc all_cases.cc)
target_link_libraries(all_cases common)
