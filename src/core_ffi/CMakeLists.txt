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
  if (CARGO_BUILD)
    install(TARGETS klotski_core DESTINATION .)
  else()
    # just for IDE highlight
    add_library(klotski_rust
      rust_ffi/adapter/short_code.cc
      rust_ffi/adapter/common_code.cc
      rust_ffi/target/cxxbridge/rust_ffi/src/bridge.rs.cc
    )
    target_include_directories(klotski_rust PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(klotski_rust PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/rust_ffi/target/cxxbridge)
    target_link_libraries(klotski_rust PRIVATE klotski_core)
  endif()
endif()
