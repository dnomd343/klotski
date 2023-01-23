cmake_minimum_required(VERSION 3.0)

include_directories(../src/klotski/all_cases)
add_executable(test_all_cases all_cases.cc)
target_link_libraries(test_all_cases all_cases gtest gtest_main)
add_test(NAME all_cases COMMAND test_all_cases)
