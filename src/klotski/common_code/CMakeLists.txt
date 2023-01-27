cmake_minimum_required(VERSION 3.0)

add_library(common_code OBJECT convert.cc serialize.cc common_code.cc)
#target_link_libraries(common_code PUBLIC utils)
