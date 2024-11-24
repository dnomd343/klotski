cmake_minimum_required(VERSION 3.12)
project(klotski_c LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLSK_C_FFI_SRC
    all_cases.cc
)

# TODO: add `-fno-rtti -fno-exceptions -fvisibility=hidden`
add_library(klotski_c STATIC ${KLSK_C_FFI_SRC})  # TODO: allow shared lib
target_include_directories(klotski_c PUBLIC include)
target_link_libraries(klotski_c PRIVATE klotski::core)

set_target_properties(klotski_c PROPERTIES OUTPUT_NAME klotski)
add_library(klotski::c_ffi ALIAS klotski_c)
