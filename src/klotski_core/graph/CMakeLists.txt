cmake_minimum_required(VERSION 3.0)

include_directories(svg)

# TODO: svg compile together
add_subdirectory(svg)

# TODO: STATIC flag
add_library(graph case.cc graph.cc)
target_link_libraries(graph PUBLIC analyse svg)

# TODO: using OBJECT instead of STATIC
# TODO: remove `target_link_libraries`
