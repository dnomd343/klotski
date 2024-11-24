cmake_minimum_required(VERSION 3.12)
project(core_ffi)

if (KLSK_C_FFI)
  add_subdirectory(c_ffi)
endif()

if (KLSK_RUST_FFI)
  add_subdirectory(rust_ffi)
endif()

if (KLSK_PYTHON_FFI)
  add_subdirectory(python_ffi)
endif()
