cmake_minimum_required(VERSION 3.0)

add_library(short_code short_code.cc)
target_link_libraries(short_code all_cases)
