cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

project(klotski-core VERSION 0.1.2 LANGUAGES CXX)

################################################################################

option(BUILD_DYN "build shared klotski core library" OFF)
if (BUILD_DYN)
    add_compile_options(-fPIC)
endif()

################################################################################

macro(git_tag _tag)
    find_package(Git QUIET)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags
                OUTPUT_VARIABLE ${_tag}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
        )
    endif()
endmacro()

macro(git_branch _branch)
    find_package(Git QUIET)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
                OUTPUT_VARIABLE ${_branch}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
        )
    endif()
endmacro()

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

################################################################################

set(GIT_TAG_LONG "")
git_tag(GIT_TAG_LONG)
if(GIT_TAG_LONG STREQUAL "")  # without git tag
    set(GIT_TAG_LONG "unknown")
endif()

set(GIT_BRANCH "")
git_branch(GIT_BRANCH)
if(GIT_BRANCH STREQUAL "")  # without git branch
    set(GIT_BRANCH "unknown")
endif()

set(GIT_COMMIT_LONG "")
git_commit_id(GIT_COMMIT_LONG)
if(GIT_COMMIT_LONG STREQUAL "")  # without git commit
    set(GIT_COMMIT_LONG "unknown")
endif()

set(PLAT "${CMAKE_SYSTEM_NAME}")
set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")

set(VERSION_MAJOR ${klotski-core_VERSION_MAJOR})
set(VERSION_MINOR ${klotski-core_VERSION_MINOR})
set(VERSION_ALTER ${klotski-core_VERSION_PATCH})

string(TIMESTAMP VERSION_BUILD "%Y-%m-%d %H:%M:%S")

set(COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

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

include_directories(group)

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

add_subdirectory(group)

################################################################################

include_directories(.)
set(FFI_SRC codec.cc all_cases.cc tmain.cc metadata.cc benchmark.cc)
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

list(APPEND OBJS $<TARGET_OBJECTS:group>)

################################################################################

if (BUILD_DYN)
    add_library(klotski SHARED ${OBJS})
else()
    add_library(klotski STATIC ${OBJS})
endif()

################################################################################
