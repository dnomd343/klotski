cmake_minimum_required(VERSION 3.12)
project(klotski_rs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

if (KLSK_DEV_MODE)
  set(KLSK_RUST_FFI_SRC
      adapter/layout.cc
      adapter/short_code.cc
      target/cxxbridge/rust_ffi/src/bridge.rs.cc
  )
  add_library(klotski_rs STATIC ${KLSK_RUST_FFI_SRC}) # just for IDE highlight
  target_include_directories(klotski_rs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
  target_include_directories(klotski_rs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/target/cxxbridge)
  target_link_libraries(klotski_rs PRIVATE klotski::core)

  set_target_properties(klotski_rs PROPERTIES OUTPUT_NAME klotski)
  add_library(klotski::rs_ffi ALIAS klotski_rs)
endif()
