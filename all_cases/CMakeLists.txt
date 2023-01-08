cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)

add_executable(klotski main.cc common.cc basic_ranges.cc all_cases.cc)
target_link_libraries(klotski pthread)
