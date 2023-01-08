cmake_minimum_required(VERSION 3.0)

include_directories(${PROJECT_SOURCE_DIR}/src/common)
include_directories(${PROJECT_SOURCE_DIR}/src/all_cases)

add_subdirectory(common)
add_subdirectory(all_cases)

add_executable(klotski main.cc)
target_link_libraries(klotski common all_cases)
