cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(${KLOTSKI_ROOT_PATH}/third_party/googletest/googletest/include)
add_subdirectory(${KLOTSKI_ROOT_PATH}/third_party/googletest googletest EXCLUDE_FROM_ALL)

#unset(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
include_directories(${KLOTSKI_ROOT_PATH}/third_party/md5)
add_library(md5 STATIC ${KLOTSKI_ROOT_PATH}/third_party/md5/md5.cpp)

set(KLOTSKI_TEST_DEPS klotski-core gtest gtest_main md5)

include_directories(${KLOTSKI_ROOT_PATH}/src/core/all_cases)
include_directories(${KLOTSKI_ROOT_PATH}/src/core/utils)


add_executable(test_klotski_cases cases/all_cases.cc)
target_link_libraries(test_klotski_cases PRIVATE ${KLOTSKI_TEST_DEPS})
add_test(NAME klotski_cases COMMAND test_klotski_cases)
