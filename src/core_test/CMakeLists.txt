cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

###############################################################################################

include_directories(${KLOTSKI_ROOT}/third_party/md5sum)
add_subdirectory(${KLOTSKI_ROOT}/third_party/md5sum md5sum EXCLUDE_FROM_ALL)

include_directories(${KLOTSKI_ROOT}/third_party/googletest/googletest/include)
add_subdirectory(${KLOTSKI_ROOT}/third_party/googletest googletest EXCLUDE_FROM_ALL)

set(KLOTSKI_TEST_DEPS klotski-core gtest gtest_main md5sum)

###############################################################################################

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
