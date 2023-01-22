cmake_minimum_required(VERSION 3.0)

include_directories(klotski)
include_directories(klotski/utils)
include_directories(klotski/all_cases)

include_directories(klotski/raw_code)
include_directories(klotski/short_code)
include_directories(klotski/common_code)

include_directories(klotski/core)
include_directories(klotski/analyse)
include_directories(klotski/fast_cal)

add_subdirectory(klotski)

add_executable(klotski_bin main.cc)
target_link_libraries(klotski_bin klotski)
