cmake_minimum_required(VERSION 3.0)

add_library(short_code convert.cc short_code.cc data_loader.cc)
target_link_libraries(short_code common all_cases)
