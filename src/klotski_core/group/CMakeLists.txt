cmake_minimum_required(VERSION 3.0)

find_package(unordered_dense CONFIG REQUIRED)

add_library(group OBJECT group.cc)

target_link_libraries(group unordered_dense::unordered_dense)
