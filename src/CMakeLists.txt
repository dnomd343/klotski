cmake_minimum_required(VERSION 3.0)

project(klotski-cli LANGUAGES C)

include_directories(klotski/ffi)

add_subdirectory(klotski)

add_executable(cli main.c)
target_link_libraries(cli PUBLIC klotski)
