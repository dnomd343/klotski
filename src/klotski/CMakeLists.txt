cmake_minimum_required(VERSION 3.0)

project(klotski-core VERSION 0.0.1 LANGUAGES CXX)

configure_file(
    utils/version.h.in
    ${PROJECT_SOURCE_DIR}/utils/version.h
)

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

include_directories(benchmark)

################################################################

add_subdirectory(utils)
add_subdirectory(all_cases)

add_subdirectory(raw_code)
add_subdirectory(short_code)
add_subdirectory(common_code)

add_subdirectory(core)
add_subdirectory(analyse)
add_subdirectory(fast_cal)

add_subdirectory(benchmark)

################################################################

set(FFI_SRC codec.cc tmain.cc version.cc)
list(TRANSFORM FFI_SRC PREPEND "ffi/")
add_library(klotski-ffi OBJECT ${FFI_SRC})

set(OBJS $<TARGET_OBJECTS:klotski-ffi>)

list(APPEND OBJS $<TARGET_OBJECTS:utils>)
list(APPEND OBJS $<TARGET_OBJECTS:all_cases>)

list(APPEND OBJS $<TARGET_OBJECTS:raw_code>)
list(APPEND OBJS $<TARGET_OBJECTS:short_code>)
list(APPEND OBJS $<TARGET_OBJECTS:common_code>)

list(APPEND OBJS $<TARGET_OBJECTS:core>)
list(APPEND OBJS $<TARGET_OBJECTS:analyse>)
list(APPEND OBJS $<TARGET_OBJECTS:fast_cal>)

list(APPEND OBJS $<TARGET_OBJECTS:benchmark>)

################################################################

option(STATIC_LIB "build static library" ON)
if (STATIC_LIB)
    add_library(klotski STATIC ${OBJS})
else()
    add_library(klotski SHARED ${OBJS})
endif()

################################################################
