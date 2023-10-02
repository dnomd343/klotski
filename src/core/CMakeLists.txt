cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

project(klotski-core VERSION 0.9.1 LANGUAGES CXX)

add_compile_options(-fno-exceptions)

#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include_directories(utils)
include_directories(all_cases)
include_directories(common_code)

add_library(${PROJECT_NAME}
        all_cases/basic_ranges.cc
        all_cases/all_cases.cc
        ffi/all_cases.cc
        common_code/common_code.cc
        common_code/serialize.cc
        common_code/convert.cc
)

add_executable(${PROJECT_NAME}_cli main.cc
        all_cases/basic_ranges.cc
        all_cases/all_cases.cc
        ffi/all_cases.cc
        common_code/common_code.cc
        common_code/serialize.cc
        common_code/convert.cc
)
