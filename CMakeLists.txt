cmake_minimum_required(VERSION 3.0)
project(klotski)

set(CMAKE_CXX_STANDARD 14)

add_executable(klotski main.cc case.cc common.cc klotski.cc)
