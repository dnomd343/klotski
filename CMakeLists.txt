cmake_minimum_required(VERSION 3.12)
project(klotski)

# ------------------------------------------------------------------------------------ #

option(KLSK_ENABLE_LTO "Enable LTO optimisation of the project." OFF)
option(KLSK_SHARED_LIB "Build klotski core as a dynamic library." OFF)
option(KLSK_ENABLE_TESTING "Enable testing of the klotski project." ON)
option(KLSK_ENABLE_BENCHMARK "Enable benchmark of the klotski project." ON)

option(KLSK_C_FFI "" ON)
option(KLSK_PYTHON_FFI "" ON)
option(KLSK_RUST_FFI "" ON)

# ------------------------------------------------------------------------------------ #

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

# TODO: enabled by LTO option
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-flto=full)
else ()
  #add_compile_options(-flto)  # TODO: only for g++
endif ()

# TODO: for python-ffi
if (SKBUILD)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

get_filename_component(KLSK_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

include(third_party/ThirdParty.cmake)

# ------------------------------------------------------------------------------------ #

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#file(
#    COPY ${PROJECT_SOURCE_DIR}/src/klotski_core/klotski.h
#    DESTINATION ${PROJECT_SOURCE_DIR}/bin
#    FOLLOW_SYMLINK_CHAIN
#)

add_subdirectory(src)

# ------------------------------------------------------------------------------------ #
