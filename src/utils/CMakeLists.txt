cmake_minimum_required(VERSION 3.0)

add_library(utils common.cc)
target_link_libraries(utils short_code)
