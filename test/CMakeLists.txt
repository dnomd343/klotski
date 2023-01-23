cmake_minimum_required(VERSION 3.0)

################################################################

enable_testing()

################################################################

include_directories(../third_party/md5)
add_library(md5 ../third_party/md5/md5.cpp)

################################################################

include_directories(../src/klotski/all_cases)
add_executable(test_all_cases all_cases.cc)
target_link_libraries(test_all_cases gtest gtest_main)
target_link_libraries(test_all_cases md5 all_cases)
add_test(NAME all_cases COMMAND test_all_cases)

################################################################
