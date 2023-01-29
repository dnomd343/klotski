cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

project(klotski-core VERSION 0.1.2 LANGUAGES CXX)

################################################################################

macro(git_commit_id _hash)
    find_package(Git QUIET)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
                OUTPUT_VARIABLE ${_hash}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
        )
    endif()
endmacro()

set(GIT_COMMIT_ID "")
git_commit_id(GIT_COMMIT_ID)

string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")

set(VERSION_MAJOR ${klotski-core_VERSION_MAJOR})
set(VERSION_MINOR ${klotski-core_VERSION_MINOR})
set(VERSION_PATCH ${klotski-core_VERSION_PATCH})

set(SYSTEM_INFO "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

set(COMPILER_INFO "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

configure_file(
    ${PROJECT_SOURCE_DIR}/utils/metadata.h.in
    ${PROJECT_SOURCE_DIR}/utils/metadata.h
)

################################################################################

include_directories(utils)
include_directories(all_cases)

include_directories(raw_code)
include_directories(short_code)
include_directories(common_code)

include_directories(core)
include_directories(analyse)
include_directories(fast_cal)

include_directories(benchmark)

################################################################################

add_subdirectory(utils)
add_subdirectory(all_cases)

add_subdirectory(raw_code)
add_subdirectory(short_code)
add_subdirectory(common_code)

add_subdirectory(core)
add_subdirectory(analyse)
add_subdirectory(fast_cal)

add_subdirectory(benchmark)

################################################################################

include_directories(.)
set(FFI_SRC codec.cc all_cases.cc tmain.cc metadata.cc)
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

################################################################################

option(BUILD_DYN "build shared klotski core library" OFF)
if (BUILD_DYN)
    add_library(klotski SHARED ${OBJS})
else()
    add_library(klotski STATIC ${OBJS})
endif()

################################################################################
