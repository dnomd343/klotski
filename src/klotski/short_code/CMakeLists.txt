cmake_minimum_required(VERSION 3.0)

include_directories(offset)

add_library(short_code OBJECT convert.cc serialize.cc short_code.cc)
#target_link_libraries(short_code PUBLIC utils all_cases)
