cmake_minimum_required(VERSION 3.0)

add_library(graph graph.cc)
target_link_libraries(graph analyse)
