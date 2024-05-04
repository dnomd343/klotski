cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(core-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLOTSKI_TEST_DEPS klotski klotski_core
        GTest::gtest_main bs::thread_pool md5sum::md5 xxHash::xxh3)

# ------------------------------------------------------------------------------------ #

include_directories(utility)

# TODO: just pass compile for now
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core/utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core/raw_code)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core/all_cases)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core/short_code)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core/common_code)

# ------------------------------------------------------------------------------------ #

set(KLOTSKI_TEST_CASES_SRC
    cases/all_cases.cc
    cases/basic_ranges.cc
)

add_executable(test_klotski_cases ${KLOTSKI_TEST_CASES_SRC})
target_link_libraries(test_klotski_cases PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_cases COMMAND test_klotski_cases)

# ------------------------------------------------------------------------------------ #

set(KLOTSKI_TEST_FFI_SRC
    ffi/all_cases.cc
)

add_executable(test_klotski_ffi ${KLOTSKI_TEST_FFI_SRC})
target_link_libraries(test_klotski_ffi PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_ffi COMMAND test_klotski_ffi)

# ------------------------------------------------------------------------------------ #

set(KLOTSKI_TEST_CODEC_SRC
    codec/mirror.cc
    codec/raw_code.cc
    codec/short_code.cc
    codec/common_code.cc
)

add_executable(test_klotski_codec ${KLOTSKI_TEST_CODEC_SRC})
target_link_libraries(test_klotski_codec PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_codec COMMAND test_klotski_codec)

# ------------------------------------------------------------------------------------ #
