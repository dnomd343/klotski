cmake_minimum_required(VERSION 3.0)

################################################################

enable_testing()

set(TEST_DEPS gtest gtest_main klotski)

################################################################

unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
include_directories(../third_party/md5)
add_library(md5 STATIC ../third_party/md5/md5.cpp)

################################################################

include_directories(../src/klotski_core/utils)
include_directories(../src/klotski_core/all_cases)

include_directories(../src/klotski_core/raw_code)
include_directories(../src/klotski_core/short_code)
include_directories(../src/klotski_core/common_code)

add_executable(test_basic utils.cc all_cases.cc)
target_link_libraries(test_basic PUBLIC ${TEST_DEPS} md5)
add_test(NAME basic COMMAND test_basic)

add_executable(test_codec short_code.cc raw_code.cc common_code.cc)
target_link_libraries(test_codec ${TEST_DEPS})
add_test(NAME codec COMMAND test_codec)

################################################################
