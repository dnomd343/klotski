cmake_minimum_required(VERSION 3.0)

include_directories(offset)

add_library(short_code STATIC convert.cc serialize.cc short_code.cc)
target_link_libraries(short_code PUBLIC utils all_cases)
