cmake_minimum_required(VERSION 3.0)

add_library(common_code common_code.cc)
target_link_libraries(common_code utils raw_code)
