cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

project(klotski-core VERSION 0.1.2 LANGUAGES CXX)

add_compile_options(-fno-exceptions)

include_directories(utils)

add_library(${PROJECT_NAME} all_cases/basic_ranges.cc all_cases/all_cases.cc)

#add_executable(${PROJECT_NAME} main.cc all_cases/basic_ranges.cc all_cases/all_cases.cc)
