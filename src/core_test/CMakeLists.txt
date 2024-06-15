cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(core-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLSK_TEST_DEPS klotski_c klotski_core
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

set(KLSK_TEST_CASES_SRC
    cases/ranges.cc
    cases/basic_ranges.cc
    cases/all_cases.cc
    cases/group_union.cc
    cases/helper/impl.cc
)

add_executable(test_klotski_cases ${KLSK_TEST_CASES_SRC})
target_link_libraries(test_klotski_cases PRIVATE ${KLSK_TEST_DEPS})
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
    codec/mirror.cc
    codec/raw_code.cc
    codec/short_code.cc
    codec/common_code.cc
)

add_executable(test_klotski_codec ${KLSK_TEST_CODEC_SRC})
target_link_libraries(test_klotski_codec PRIVATE ${KLSK_TEST_DEPS})
add_test(NAME klotski_codec COMMAND test_klotski_codec)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_CORE_SRC
    core/core.cc
)

add_executable(test_klotski_core ${KLSK_TEST_CORE_SRC})
target_link_libraries(test_klotski_core PRIVATE ${KLSK_TEST_DEPS})
add_test(NAME klotski_core COMMAND test_klotski_core)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_GROUP_TMP_SRC
    group_tmp/group_union.cc
    group_tmp/group_extend.cc
)

add_executable(test_klotski_group_tmp ${KLSK_TEST_GROUP_TMP_SRC})
target_link_libraries(test_klotski_group_tmp PRIVATE ${KLSK_TEST_DEPS})
add_test(NAME klotski_group_tmp COMMAND test_klotski_group_tmp)

# ------------------------------------------------------------------------------------ #
