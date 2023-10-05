cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

set(KLOTSKI_TEST_DEPS klotski-core gtest gtest_main md5sum xxHash::xxhash)

###############################################################################################

include_directories(utils)

include_directories(${KLOTSKI_ROOT}/src/core/ffi)
include_directories(${KLOTSKI_ROOT}/src/core/utils)
include_directories(${KLOTSKI_ROOT}/src/core/all_cases)

include_directories(${KLOTSKI_ROOT}/src/core/raw_code)
include_directories(${KLOTSKI_ROOT}/src/core/short_code)
include_directories(${KLOTSKI_ROOT}/src/core/common_code)

###############################################################################################

set(KLOTSKI_TEST_CASES_SRC
    cases/all_cases.cc
)

add_executable(test_klotski_cases ${KLOTSKI_TEST_CASES_SRC})
target_link_libraries(test_klotski_cases PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_cases COMMAND test_klotski_cases)

###############################################################################################

set(KLOTSKI_TEST_FFI_SRC
    ffi/all_cases.cc
)

add_executable(test_klotski_ffi ${KLOTSKI_TEST_FFI_SRC})
target_link_libraries(test_klotski_ffi PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_ffi COMMAND test_klotski_ffi)

###############################################################################################

set(KLOTSKI_TEST_CODEC_SRC
    codec/mirror.cc
    codec/raw_code.cc
    codec/short_code.cc
    codec/common_code.cc
)

add_executable(test_klotski_codec ${KLOTSKI_TEST_CODEC_SRC})
target_link_libraries(test_klotski_codec PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_codec COMMAND test_klotski_codec)

###############################################################################################
