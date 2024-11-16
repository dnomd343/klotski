cmake_minimum_required(VERSION 3.12)
project(py_ffi)

set(CMAKE_CXX_STANDARD 23)

pybind11_add_module(klotski_py
    klotski.cc

    binder/cases.cc
    binder/common_code.cc
    binder/short_code.cc
    binder/group_union.cc
    binder/group.cc
    binder/fast_cal.cc

    wrapper/short_code.cc
    wrapper/common_codec.cc
    wrapper/cases.cc
    wrapper/group_union.cc
    wrapper/group.cc
    wrapper/fast_cal.cc
)
target_include_directories(klotski_py PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(klotski_py PRIVATE klotski::core)
set_target_properties(klotski_py PROPERTIES OUTPUT_NAME _klotski)

if (SKBUILD)
  install(TARGETS klotski_py DESTINATION .)
endif()
