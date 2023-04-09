cmake_minimum_required(VERSION 3.0)

project(klotski)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

################################################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(
    COPY ${PROJECT_SOURCE_DIR}/src/klotski_core/klotski.h
    DESTINATION ${PROJECT_SOURCE_DIR}/bin
    FOLLOW_SYMLINK_CHAIN
)

################################################################################

option(BUILD_DYN "build shared klotski core library" OFF)
if (BUILD_DYN)
    add_compile_options(-fPIC)
endif()

set(ABSL_PROPAGATE_CXX_STD ON)
include_directories(third_party/abseil-cpp)
add_subdirectory(third_party/abseil-cpp)

add_subdirectory(src)

################################################################################

option(CORE_TEST "build test for klotski core" ON)
#if (CORE_TEST)
    include_directories(third_party/googletest/googletest/include)
    add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
    add_subdirectory(test)
#endif()

################################################################################
