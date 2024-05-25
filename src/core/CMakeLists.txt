cmake_minimum_required(VERSION 3.12)
project(core VERSION 0.2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLOTSKI_CORE_SRC
        all_cases/internal/basic_ranges.cc
        all_cases/internal/all_cases.cc

        common_code/internal/common_code.cc
        common_code/internal/serialize.cc

        raw_code/internal/raw_code.cc
        raw_code/internal/convert.cc
        raw_code/internal/mirror.cc

        short_code/internal/convert.cc
        short_code/internal/serialize.cc

        core/internal/core.cc

        fast_cal/internal/cal_core.cc
        fast_cal/internal/fast_cal.cc

        group/internal/group.cc
)

add_library(klotski_core STATIC ${KLOTSKI_CORE_SRC})
target_compile_options(klotski_core PRIVATE -fno-rtti -fno-exceptions)  # option for `-fvisibility=hidden`
target_include_directories(klotski_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(klotski_core PRIVATE absl::flat_hash_map)
add_library(klotski::core ALIAS klotski_core)

# TODO: just for dev testing
add_executable(klotski_core_bin main.cc)
target_link_libraries(klotski_core_bin PRIVATE klotski_core)

add_executable(codec_benchmark benchmark/codec.cc)
target_compile_options(codec_benchmark PRIVATE -fno-rtti -fno-exceptions)
target_link_libraries(codec_benchmark PRIVATE klotski::core benchmark::benchmark_main)

add_executable(group_benchmark benchmark/group.cc)
target_compile_options(group_benchmark PRIVATE -fno-rtti -fno-exceptions)
target_link_libraries(group_benchmark PRIVATE klotski::core benchmark::benchmark_main)
