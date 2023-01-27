cmake_minimum_required(VERSION 3.0)

################################################################

enable_testing()

################################################################

include_directories(../third_party/md5)
add_library(md5 STATIC ../third_party/md5/md5.cpp)

################################################################

include_directories(../src/klotski/utils)
add_executable(test_utils utils.cc)
target_link_libraries(test_utils PUBLIC gtest gtest_main)
target_link_libraries(test_utils PUBLIC klotski)
add_test(NAME utils COMMAND test_utils)

################################################################

include_directories(../src/klotski/all_cases)
add_executable(test_all_cases all_cases.cc)
target_link_libraries(test_all_cases PUBLIC gtest gtest_main)
target_link_libraries(test_all_cases PUBLIC klotski md5)
add_test(NAME all_cases COMMAND test_all_cases)

################################################################
