cmake_minimum_required(VERSION 3.12)
project(klotski_py LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLSK_PYTHON_FFI_SRC
    klotski.cc

    binder/klsk_cases.cc
    binder/klsk_code.cc
    binder/group_union.cc
    binder/group.cc
    binder/fast_cal.cc

    wrapper/short_code.cc
    wrapper/layout.cc
    wrapper/cases.cc
    wrapper/group_union.cc
    wrapper/group.cc
    wrapper/fast_cal.cc
    wrapper/speed_up.cc
)

pybind11_add_module(klotski_py ${KLSK_PYTHON_FFI_SRC})
target_include_directories(klotski_py PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(klotski_py PRIVATE klotski::core)

set_target_properties(klotski_py PROPERTIES OUTPUT_NAME _klotski)
add_library(klotski::py_ffi ALIAS klotski_py)

if (SKBUILD)
  install(TARGETS klotski_py DESTINATION .)
endif()
