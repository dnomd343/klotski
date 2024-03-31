cmake_minimum_required(VERSION 3.12)
project(core VERSION 0.2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLOTSKI_CORE_SRC
        all_cases/basic_ranges.cc
        all_cases/all_cases.cc
        common_code/common_code.cc
        common_code/serialize.cc
        common_code/sundry.cc
        raw_code/raw_code.cc
        raw_code/convert.cc
        raw_code/sundry.cc
        raw_code/mirror.cc
        short_code/convert.cc
        short_code/serialize.cc
        short_code/short_code.cc
        short_code/sundry.cc
)
add_library(klotski_core STATIC ${KLOTSKI_CORE_SRC})
target_compile_options(klotski_core PRIVATE -fno-rtti -fno-exceptions)
target_include_directories(klotski_core PUBLIC utils all_cases raw_code short_code common_code)

# TODO: just for dev testing
add_executable(klotski_core_bin main.cc)
target_link_libraries(klotski_core_bin PRIVATE klotski_core)
