cmake_minimum_required(VERSION 3.0)

project(klotski-cli LANGUAGES C)

add_subdirectory(klotski_core)
include_directories(klotski_core)

add_executable(cli main.c)
target_link_libraries(cli PRIVATE klotski)
