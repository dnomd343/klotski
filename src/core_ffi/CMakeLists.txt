cmake_minimum_required(VERSION 3.12)
project(core_ffi)

set(CMAKE_CXX_STANDARD 23)

if (KLSK_C_FFI)
    add_library(klotski_c c_ffi/all_cases.cc)
    target_include_directories(klotski_c PUBLIC c_ffi/include)
    target_link_libraries(klotski_c PRIVATE klotski::core)
    set_target_properties(klotski_c PROPERTIES OUTPUT_NAME klotski)
endif()

if (KLSK_PYTHON_FFI)
    pybind11_add_module(klotski_py
        py_ffi/binder.cc
        py_ffi/binder/cases.cc

        py_ffi/wrapper/short_code.cc
        py_ffi/wrapper/common_codec.cc
        py_ffi/wrapper/cases.cc
        py_ffi/wrapper/group_union.cc
        py_ffi/wrapper/group.cc
    )
    target_include_directories(klotski_py PRIVATE py_ffi)
    target_link_libraries(klotski_py PRIVATE klotski::core)
    set_target_properties(klotski_py PROPERTIES OUTPUT_NAME klotski)
endif()

if (KLSK_RUST_FFI)
    # just for IDE highlight
    add_library(klotski_rust
        rust_ffi/src/demo.cc
        rust_ffi/target/cxxbridge/klotski/src/common_code.rs.cc
    )
    target_include_directories(klotski_rust PRIVATE rust_ffi/target/cxxbridge)
endif()
