cmake_minimum_required(VERSION 3.12)

project(klotski_py_demo VERSION "0.0.1")

add_subdirectory(src/project)

install(TARGETS klotski_py DESTINATION .)
