cmake_minimum_required(VERSION 3.0)

project(klotski)
#set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(
    COPY ${PROJECT_SOURCE_DIR}/src/klotski_core/klotski.h
    DESTINATION ${PROJECT_SOURCE_DIR}/bin
    FOLLOW_SYMLINK_CHAIN
)

# TODO: only in libklotski.so
add_compile_options(-fPIC)

add_subdirectory(src)

################################################

include_directories(third_party/googletest/googletest/include)
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
add_subdirectory(test)

################################################
