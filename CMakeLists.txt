cmake_minimum_required(VERSION 3.12)
project(klotski)

# ------------------------------------------------------------------------------------ #

option(KLSK_ENABLE_LTO "Enable LTO optimisation of the project." ON)
#option(KLSK_SHARED_LIB "Build klotski core as a dynamic library." OFF)
option(KLSK_ENABLE_TESTING "Enable testing of the klotski project." OFF)
option(KLSK_ENABLE_BENCHMARK "Enable benchmark of the klotski project." OFF)

option(KLSK_C_FFI "" OFF)
option(KLSK_RUST_FFI "" OFF)
option(KLSK_PYTHON_FFI "" OFF)

if (KLSK_DEV_MODE)
  set(KLSK_C_FFI ON)
  set(KLSK_RUST_FFI ON)
  set(KLSK_PYTHON_FFI ON)

  set(KLSK_ENABLE_LTO ON)
  set(KLSK_ENABLE_TESTING ON)
  set(KLSK_ENABLE_BENCHMARK ON)
endif()

# ------------------------------------------------------------------------------------------- #

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -g)

if (KLSK_ENABLE_LTO)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "klotski enable LTO for clang")
    add_compile_options(-flto=full)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # add_compile_options(-flto=auto)
  else()
    message(FATAL_ERROR "unsupported compiler")  # TODO: add MSVC support
  endif()
endif()

# TODO: add `KLSK_ENABLE_PIC` option
if (SKBUILD)  # for python-ffi
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

get_filename_component(KLSK_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

include(third_party/ThirdParty.cmake)

# ------------------------------------------------------------------------------------------- #

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#file(
#    COPY ${PROJECT_SOURCE_DIR}/src/klotski_core/klotski.h
#    DESTINATION ${PROJECT_SOURCE_DIR}/bin
#    FOLLOW_SYMLINK_CHAIN
#)

add_subdirectory(src)

# ------------------------------------------------------------------------------------------- #
