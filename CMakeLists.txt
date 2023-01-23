cmake_minimum_required(VERSION 3.0)

project(klotski)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# TODO: only in libklotski.so
add_compile_options(-fPIC)

add_subdirectory(src)

################################################

enable_testing()
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
include_directories(third_party/googletest/googletest/include)
add_subdirectory(test)

################################################
