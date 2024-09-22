cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(core-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLSK_TEST_DEPS klotski_c klotski_core
        GTest::gtest_main bs::thread_pool md5sum::md5 xxHash::xxh3)

# ------------------------------------------------------------------------------------ #

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ------------------------------------------------------------------------------------ #

add_library(test_helper
    helper/internal/parallel.cc
)
target_link_libraries(test_helper PRIVATE klotski_core bs::thread_pool)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_CASES_SRC
    cases/ranges.cc
    cases/ranges_union.cc
    cases/basic_ranges.cc
    cases/all_cases.cc
    cases/group_union.cc
    cases/group.cc
    cases/helper/block_num.cc
    cases/helper/group_impl.cc
)

add_executable(test_klotski_cases ${KLSK_TEST_CASES_SRC})
target_link_libraries(test_klotski_cases PRIVATE ${KLSK_TEST_DEPS} test_helper)
add_test(NAME klotski_cases COMMAND test_klotski_cases)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_FFI_SRC
    ffi/all_cases.cc
)

add_executable(test_klotski_ffi ${KLSK_TEST_FFI_SRC})
target_link_libraries(test_klotski_ffi PRIVATE ${KLSK_TEST_DEPS})
add_test(NAME klotski_ffi COMMAND test_klotski_ffi)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_CODEC_SRC
    codec/raw_code.cc
    codec/short_code.cc
    codec/common_code.cc
)

add_executable(test_klotski_codec ${KLSK_TEST_CODEC_SRC})
target_link_libraries(test_klotski_codec PRIVATE ${KLSK_TEST_DEPS} test_helper)
add_test(NAME klotski_codec COMMAND test_klotski_codec)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_CORE_SRC
    core/core.cc
)

add_executable(test_klotski_core ${KLSK_TEST_CORE_SRC})
target_link_libraries(test_klotski_core PRIVATE ${KLSK_TEST_DEPS})
add_test(NAME klotski_core COMMAND test_klotski_core)

# ------------------------------------------------------------------------------------ #
