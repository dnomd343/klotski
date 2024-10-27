cmake_minimum_required(VERSION 3.12)
project(core VERSION 0.2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLOTSKI_CORE_SRC
        all_cases/internal/basic_ranges.cc
        all_cases/internal/all_cases.cc

        common_code/internal/common_code.cc
        common_code/internal/serialize.cc
        common_code/internal/mirror.cc

        raw_code/internal/raw_code.cc
        raw_code/internal/convert.cc
        raw_code/internal/mirror.cc

        short_code/internal/convert.cc
        short_code/internal/serialize.cc

        mover/internal/mover.cc

        fast_cal/internal/demo.cc

        group/internal/group_union.cc
        group/internal/extend.cc
        group/internal/group.cc
        group/internal/group_cases.cc

        ranges/internal/spawn.cc
        ranges/internal/ranges.cc
        ranges/internal/derive.cc
)

add_library(klotski_core STATIC ${KLOTSKI_CORE_SRC})
target_compile_options(klotski_core PRIVATE -fno-rtti -fno-exceptions)  # option for `-fvisibility=hidden`
target_include_directories(klotski_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(klotski_core PRIVATE absl::flat_hash_map)
target_link_libraries(klotski_core PUBLIC phmap)
add_library(klotski::core ALIAS klotski_core)

# TODO: just for dev testing
add_executable(klotski_core_bin main.cc)
target_link_libraries(klotski_core_bin PRIVATE klotski_core)

add_executable(codec_benchmark benchmark/codec.cc)
target_compile_options(codec_benchmark PRIVATE -fno-rtti -fno-exceptions -fno-access-control)
target_link_libraries(codec_benchmark PRIVATE klotski::core benchmark::benchmark_main)

add_executable(group_benchmark benchmark/group.cc)
target_compile_options(group_benchmark PRIVATE -fno-rtti -fno-exceptions -fno-access-control)
target_link_libraries(group_benchmark PRIVATE klotski::core benchmark::benchmark_main)

add_executable(bm_all_cases benchmark/all_cases.cc)
target_compile_options(bm_all_cases PRIVATE -fno-rtti -fno-exceptions -fno-access-control)
target_link_libraries(bm_all_cases PRIVATE klotski::core benchmark::benchmark_main bs::thread_pool)

add_executable(bm_ranges benchmark/ranges.cc)
target_compile_options(bm_ranges PRIVATE -fno-rtti -fno-exceptions)
target_link_libraries(bm_ranges PRIVATE klotski::core benchmark::benchmark_main)

add_executable(bm_utility benchmark/utility.cc)
target_compile_options(bm_utility PRIVATE -fno-rtti -fno-exceptions)
target_link_libraries(bm_utility PRIVATE klotski::core benchmark::benchmark_main)

add_executable(bm_fast_cal benchmark/fast_cal.cc)
target_compile_options(bm_fast_cal PRIVATE -fno-rtti -fno-exceptions)
target_link_libraries(bm_fast_cal PRIVATE klotski::core benchmark::benchmark_main)
