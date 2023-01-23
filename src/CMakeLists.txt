cmake_minimum_required(VERSION 3.0)

include_directories(klotski)
include_directories(benchmark)

add_subdirectory(klotski)
add_subdirectory(benchmark)

add_executable(klotski_bin main.cc)
target_link_libraries(klotski_bin benchmark)
#target_link_libraries(klotski_bin klotski)
