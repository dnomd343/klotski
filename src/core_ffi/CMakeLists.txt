cmake_minimum_required(VERSION 3.12)
project(core_ffi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(klotski all_cases.cc)
target_include_directories(klotski PUBLIC include)
target_link_libraries(klotski PRIVATE klotski_core)
