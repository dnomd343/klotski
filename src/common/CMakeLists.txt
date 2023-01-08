cmake_minimum_required(VERSION 3.0)

add_library(common common.cc)
target_link_libraries(common short_code)
