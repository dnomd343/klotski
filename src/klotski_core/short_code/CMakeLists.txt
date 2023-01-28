cmake_minimum_required(VERSION 3.0)

include_directories(offset)

add_library(short_code OBJECT convert.cc serialize.cc short_code.cc)
