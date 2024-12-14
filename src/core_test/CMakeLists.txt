cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(klotski_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLSK_TEST_DEPS klotski::core GTest::gtest_main bs::thread_pool)

# ------------------------------------------------------------------------------------ #

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ------------------------------------------------------------------------------------ #

add_library(test_helper
    helper/internal/block_num.cc
    helper/internal/concurrent.cc
    helper/internal/parallel.cc
    helper/internal/hash.cc
    helper/internal/group.cc
    helper/internal/mirror.cc
)
target_link_libraries(test_helper PRIVATE klotski_core bs::thread_pool md5sum::md5 xxHash::xxh3)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_CASES_SRC
    cases/ranges.cc
    cases/ranges_union.cc
    cases/basic_ranges.cc
    cases/all_cases.cc
)

add_executable(test_klotski_cases ${KLSK_TEST_CASES_SRC})
target_link_libraries(test_klotski_cases PRIVATE ${KLSK_TEST_DEPS} test_helper)
add_test(NAME klotski_cases COMMAND test_klotski_cases)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_GROUP_SRC
    group/group_union.cc
    group/group_legacy.cc
    group/group_pro.cc
    group/helper/group_impl.cc
)

add_executable(test_klotski_group ${KLSK_TEST_GROUP_SRC})
target_link_libraries(test_klotski_group PRIVATE ${KLSK_TEST_DEPS} test_helper)
add_test(NAME klotski_group COMMAND test_klotski_group)

# ------------------------------------------------------------------------------------ #

set(KLSK_TEST_FFI_SRC
    ffi/all_cases.cc
)

#add_executable(test_klotski_ffi ${KLSK_TEST_FFI_SRC})
#target_link_libraries(test_klotski_ffi PRIVATE ${KLSK_TEST_DEPS})
#add_test(NAME klotski_ffi COMMAND test_klotski_ffi)

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

set(KLSK_TEST_MOVER_SRC
    mover/mover.cc
)

add_executable(test_klotski_mover ${KLSK_TEST_MOVER_SRC})
target_link_libraries(test_klotski_mover PRIVATE ${KLSK_TEST_DEPS} test_helper)
add_test(NAME klotski_mover COMMAND test_klotski_mover)

# ------------------------------------------------------------------------------------ #
