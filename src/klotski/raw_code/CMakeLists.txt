cmake_minimum_required(VERSION 3.0)

add_library(raw_code STATIC convert.cc raw_code.cc)
target_link_libraries(raw_code PUBLIC utils)
