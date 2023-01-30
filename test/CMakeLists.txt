cmake_minimum_required(VERSION 3.0)

################################################################################

enable_testing()

set(TEST_DEPS gtest gtest_main klotski)

################################################################################

unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
include_directories(../third_party/md5)
add_library(md5 STATIC ../third_party/md5/md5.cpp)

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
