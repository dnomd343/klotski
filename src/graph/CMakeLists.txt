cmake_minimum_required(VERSION 3.0)

include_directories(svg)

add_subdirectory(svg)

add_library(graph case.cc graph.cc)
target_link_libraries(graph analyse svg)
