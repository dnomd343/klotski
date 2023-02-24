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

################################################################################

include_directories(../src/klotski_core/utils)
include_directories(../src/klotski_core/all_cases)

set(TEST_BASIC_SRC utils.cc all_cases.cc)
list(TRANSFORM TEST_BASIC_SRC PREPEND "basic/")
add_executable(test_basic ${TEST_BASIC_SRC})
target_link_libraries(test_basic PUBLIC ${TEST_DEPS} md5)
add_test(NAME basic COMMAND test_basic)

################################################################################

include_directories(../src/klotski_core/raw_code)
include_directories(../src/klotski_core/short_code)
include_directories(../src/klotski_core/common_code)

set(TEST_CODEC_SRC short_code.cc raw_code.cc common_code.cc)
list(TRANSFORM TEST_CODEC_SRC PREPEND "codec/")
add_executable(test_codec ${TEST_CODEC_SRC})
target_link_libraries(test_codec ${TEST_DEPS})
add_test(NAME codec COMMAND test_codec)

################################################################################

include_directories(../src/klotski_core)
include_directories(../src/klotski_core/utils)

set(TEST_FFI_SRC metadata.cc codec.cc all_cases.cc)
list(TRANSFORM TEST_FFI_SRC PREPEND "ffi/")
add_executable(test_ffi ${TEST_FFI_SRC})
target_link_libraries(test_ffi ${TEST_DEPS})
add_test(NAME ffi COMMAND test_ffi)

################################################################################

include_directories(../src/klotski_core)
add_library(test_global_utils STATIC global/global_utils.cc)
set(TEST_GLOBAL_DEPS ${TEST_DEPS} test_global_utils tiny_pool)

include_directories(../src/klotski_core/raw_code)
include_directories(../src/klotski_core/short_code)
include_directories(../src/klotski_core/common_code)

add_executable(test_codec_global global/short_code.cc global/common_code.cc global/raw_code.cc)
target_link_libraries(test_codec_global ${TEST_GLOBAL_DEPS})
add_test(NAME codec_global COMMAND test_codec_global)

#include_directories(../src/klotski_core/common_code)
#add_executable(test_global_common_code global/common_code.cc)
#target_link_libraries(test_global_common_code ${TEST_GLOBAL_DEPS})
#add_test(NAME global_common_code COMMAND test_global_common_code)
#
#include_directories(../src/klotski_core/short_code)
#add_executable(test_global_short_code global/short_code.cc)
#target_link_libraries(test_global_short_code ${TEST_GLOBAL_DEPS})
#add_test(NAME global_short_code COMMAND test_global_short_code)
#
#include_directories(../src/klotski_core/raw_code)
#add_executable(test_global_raw_code global/raw_code.cc)
#target_link_libraries(test_global_raw_code ${TEST_GLOBAL_DEPS})
#add_test(NAME global_raw_code COMMAND test_global_raw_code)

################################################################################
