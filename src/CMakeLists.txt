cmake_minimum_required(VERSION 3.0)

include_directories(klotski/ffi)

add_subdirectory(klotski)

add_executable(demo main.c)
target_link_libraries(demo PUBLIC klotski)
