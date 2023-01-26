cmake_minimum_required(VERSION 3.0)

project(klotski-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

################################################################

include_directories(utils)
include_directories(all_cases)

include_directories(raw_code)
include_directories(short_code)
include_directories(common_code)

include_directories(core)
include_directories(analyse)
include_directories(fast_cal)

################################################################

add_subdirectory(utils)
add_subdirectory(all_cases)

add_subdirectory(raw_code)
add_subdirectory(short_code)
add_subdirectory(common_code)

add_subdirectory(core)
add_subdirectory(analyse)
add_subdirectory(fast_cal)

################################################################

add_library(klotski-core OBJECT ffi/codec.cc ffi/tmain.cc)

target_link_libraries(klotski-core utils)
target_link_libraries(klotski-core all_cases)

target_link_libraries(klotski-core raw_code)
target_link_libraries(klotski-core short_code)
target_link_libraries(klotski-core common_code)

target_link_libraries(klotski-core core)
target_link_libraries(klotski-core analyse)
target_link_libraries(klotski-core fast_cal)

################################################################

#add_library(klotski SHARED $<TARGET_OBJECTS:klotski-core>)
add_library(klotski STATIC $<TARGET_OBJECTS:klotski-core>)

################################################################
