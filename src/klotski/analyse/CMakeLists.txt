cmake_minimum_required(VERSION 3.0)

add_library(analyse STATIC analyse.cc backtrack.cc)
target_link_libraries(analyse PUBLIC core)
