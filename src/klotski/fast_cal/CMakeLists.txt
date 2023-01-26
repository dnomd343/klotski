cmake_minimum_required(VERSION 3.0)

add_library(fast_cal STATIC cal_core.cc fast_cal.cc)
target_link_libraries(fast_cal PUBLIC core)
