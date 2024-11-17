cmake_minimum_required(VERSION 3.12)
project(core VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(KLOTSKI_CORE_SRC
    ranges/internal/spawn.cc
    ranges/internal/ranges.cc
    ranges/internal/derive.cc

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

    group/internal/group_union.cc
    group/internal/group.cc
    group/internal/group_cases.cc

    fast_cal/internal/fast_cal.cc
)

add_library(klotski_core STATIC ${KLOTSKI_CORE_SRC})
target_compile_options(klotski_core PRIVATE -fno-rtti -fno-exceptions)  # option for `-fvisibility=hidden`
target_include_directories(klotski_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(klotski_core PUBLIC phmap::phmap)
add_library(klotski::core ALIAS klotski_core)

# TODO: just for dev testing
if (KLSK_DEV_MODE)
  add_executable(klotski_core_bin main.cc)
  target_link_libraries(klotski_core_bin PRIVATE klotski_core)
endif()

if (KLSK_ENABLE_BENCHMARK)
  set(KLSK_BENCHMARK_OPTS -fno-rtti -fno-exceptions -fno-access-control)
  set(KLSK_BENCHMARK_LIBS klotski::core benchmark::benchmark_main bs::thread_pool)

  add_executable(bm_klsk_codec benchmark/codec.cc)
  target_compile_options(bm_klsk_codec PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_codec PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_group benchmark/group.cc)
  target_compile_options(bm_klsk_group PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_group PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_all_cases benchmark/all_cases.cc)
  target_compile_options(bm_klsk_all_cases PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_all_cases PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_ranges benchmark/ranges.cc)
  target_compile_options(bm_klsk_ranges PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_ranges PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_utility benchmark/utility.cc)
  target_compile_options(bm_klsk_utility PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_utility PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_fast_cal benchmark/fast_cal.cc)
  target_compile_options(bm_klsk_fast_cal PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_fast_cal PRIVATE ${KLSK_BENCHMARK_LIBS})
endif()
