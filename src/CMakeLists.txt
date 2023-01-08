cmake_minimum_required(VERSION 3.0)

include_directories(common)
include_directories(all_cases)
include_directories(short_code)
include_directories(common_code)

add_subdirectory(common)
add_subdirectory(all_cases)
add_subdirectory(short_code)
add_subdirectory(common_code)

add_executable(klotski main.cc)
target_link_libraries(klotski all_cases)
target_link_libraries(klotski short_code)
target_link_libraries(klotski common_code)

target_link_libraries(klotski pthread)
