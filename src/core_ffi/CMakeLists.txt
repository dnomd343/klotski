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
    add_subdirectory(py_ffi)
endif()

if (KLSK_RUST_FFI)
    # just for IDE highlight
    add_library(klotski_rust
        rust_ffi/src/demo.cc
        rust_ffi/target/cxxbridge/klotski/src/common_code.rs.cc
    )
    target_include_directories(klotski_rust PRIVATE rust_ffi/target/cxxbridge)
endif()
