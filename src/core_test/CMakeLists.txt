cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

set(KLOTSKI_TEST_DEPS klotski-core gtest gtest_main xxHash::xxhash)

###############################################################################################

include_directories(utils)

include_directories(${KLOTSKI_ROOT}/src/core/utils)
include_directories(${KLOTSKI_ROOT}/src/core/all_cases)

###############################################################################################

set(KLOTSKI_TEST_CASES_SRC
    cases/all_cases.cc
)

add_executable(test_klotski_cases ${KLOTSKI_TEST_CASES_SRC})
target_link_libraries(test_klotski_cases PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_cases COMMAND test_klotski_cases)

###############################################################################################
