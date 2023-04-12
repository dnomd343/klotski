cmake_minimum_required(VERSION 3.0)

################################################################################

enable_testing()
set(TEST_DEPS gtest gtest_main klotski)

################################################################################

unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
include_directories(../third_party/md5)
add_library(md5 STATIC ../third_party/md5/md5.cpp)

################################################################################

unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
include_directories(../third_party/tiny_thread_pool)
add_library(tiny_pool STATIC ../third_party/tiny_thread_pool/tiny_pool.c)

# TODO: tiny_pool as cpp module

################################################################################

include_directories(../src/klotski_core)
include_directories(../src/klotski_core/utils)
include_directories(../src/klotski_core/all_cases)

include_directories(../src/klotski_core/raw_code)
include_directories(../src/klotski_core/short_code)
include_directories(../src/klotski_core/common_code)

include_directories(../src/klotski_core/core)
include_directories(../src/klotski_core/group)

################################################################################

set(TEST_BASIC_SRC
    basic/utils.cc
    basic/all_cases.cc
)
add_executable(test_basic ${TEST_BASIC_SRC})
target_link_libraries(test_basic PUBLIC ${TEST_DEPS} md5)
add_test(NAME basic COMMAND test_basic)

################################################################################

set(TEST_CODEC_SRC
    codec/short_code.cc
    codec/raw_code.cc
    codec/common_code.cc
)
add_executable(test_codec ${TEST_CODEC_SRC})
target_link_libraries(test_codec ${TEST_DEPS})
add_test(NAME codec COMMAND test_codec)

################################################################################

set(TEST_CORE_SRC core/core.cc)
add_executable(test_core ${TEST_CORE_SRC})
target_link_libraries(test_core ${TEST_DEPS} absl::flat_hash_map)
add_test(NAME core COMMAND test_core)

################################################################################

set(TEST_GROUP_SRC
    group/block_num.cc
    group/build_cases.cc
)
add_executable(test_group ${TEST_GROUP_SRC})
target_link_libraries(test_group ${TEST_DEPS} md5 absl::flat_hash_map)
add_test(NAME group COMMAND test_group)

################################################################################

set(TEST_FFI_SRC
    ffi/metadata.cc
    ffi/codec.cc
    ffi/all_cases.cc
)
add_executable(test_ffi ${TEST_FFI_SRC})
target_link_libraries(test_ffi ${TEST_DEPS})
add_test(NAME ffi COMMAND test_ffi)

################################################################################

# TODO: update cmake configure

add_library(test_global_utils STATIC global/global_utils.cc)
set(TEST_GLOBAL_DEPS ${TEST_DEPS} test_global_utils tiny_pool)

add_executable(test_codec_global global/short_code.cc global/common_code.cc global/raw_code.cc)
target_link_libraries(test_codec_global ${TEST_GLOBAL_DEPS})
add_test(NAME codec_global COMMAND test_codec_global)

################################################################################
