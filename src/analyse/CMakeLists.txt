cmake_minimum_required(VERSION 3.0)

add_library(analyse analyse.cc)
target_link_libraries(analyse core)
