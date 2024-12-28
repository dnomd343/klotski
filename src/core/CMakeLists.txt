cmake_minimum_required(VERSION 3.12)
project(klotski_core VERSION 0.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

macro(git_tag _tag)
  find_package(Git QUIET)
  if (GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags
                    OUTPUT_VARIABLE ${_tag}
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
  endif()
endmacro()

macro(git_branch _branch)
  find_package(Git QUIET)
  if (GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
                    OUTPUT_VARIABLE ${_branch}
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
  endif()
endmacro()

macro(git_commit_id _hash)
  find_package(Git QUIET)
  if (GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
                    OUTPUT_VARIABLE ${_hash}
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
  endif()
endmacro()

set(GIT_TAG_LONG "")
git_tag(GIT_TAG_LONG)
if(GIT_TAG_LONG STREQUAL "")  # without git tag
    set(GIT_TAG_LONG "unknown")
endif()

set(GIT_BRANCH "")
git_branch(GIT_BRANCH)
if(GIT_BRANCH STREQUAL "")  # without git branch
    set(GIT_BRANCH "unknown")
endif()

set(GIT_COMMIT_LONG "")
git_commit_id(GIT_COMMIT_LONG)
if(GIT_COMMIT_LONG STREQUAL "")  # without git commit
    set(GIT_COMMIT_LONG "unknown")
endif()

set(PLAT "${CMAKE_SYSTEM_NAME}")  # TODO: add `KLSK_` prefix
set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")

set(VERSION_MAJOR ${klotski_core_VERSION_MAJOR})
set(VERSION_MINOR ${klotski_core_VERSION_MINOR})
set(VERSION_ALTER ${klotski_core_VERSION_PATCH})

string(TIMESTAMP VERSION_BUILD "%Y-%m-%d %H:%M:%S")

set(COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

#configure_file(
#    ${PROJECT_SOURCE_DIR}/utils/metadata.h.in
#    ${PROJECT_SOURCE_DIR}/utils/metadata.h
#)

set(KLSK_CORE_SRC
    ranges/internal/spawn.cc
    ranges/internal/ranges.cc

    all_cases/internal/basic_ranges.cc
    all_cases/internal/all_cases.cc

#    common_code/internal/common_code.cc
#    common_code/internal/serialize.cc
#    common_code/internal/mirror.cc

#    raw_code/internal/raw_code.cc
#    raw_code/internal/convert.cc
#    raw_code/internal/mirror.cc

    short_code/internal/convert.cc
#    short_code/internal/serialize.cc

    mover/internal/mover.cc

    group/internal/group_union.cc
    group/internal/group.cc
    group/internal/group_cases.cc

    fast_cal/internal/fast_cal.cc

    analyse/analyse.cc
    analyse/backtrack.cc
)

add_library(klotski_core STATIC ${KLSK_CORE_SRC})
target_compile_options(klotski_core PRIVATE -fno-rtti -fno-exceptions)  # option for `-fvisibility=hidden`
target_include_directories(klotski_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(klotski_core PUBLIC phmap::phmap)
add_library(klotski::core ALIAS klotski_core)

if (KLSK_DEV_MODE)
  add_executable(klotski_core_bin main.cc)
  target_compile_options(klotski_core_bin PRIVATE -fno-rtti -fno-exceptions)
  target_link_libraries(klotski_core_bin PRIVATE klotski::core)
endif()

if (KLSK_ENABLE_BENCHMARK)
  set(KLSK_BENCHMARK_OPTS -fno-rtti -fno-exceptions -fno-access-control)
  set(KLSK_BENCHMARK_LIBS klotski::core benchmark::benchmark_main bs::thread_pool)

  add_executable(bm_klsk_codec benchmark/codec.cc)
  target_compile_options(bm_klsk_codec PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_codec PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_common_code benchmark/common_code.cc)
  target_compile_options(bm_klsk_common_code PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_common_code PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_raw_code benchmark/raw_code.cc)
  target_compile_options(bm_klsk_raw_code PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_raw_code PRIVATE ${KLSK_BENCHMARK_LIBS})

  add_executable(bm_klsk_short_code benchmark/short_code.cc)
  target_compile_options(bm_klsk_short_code PRIVATE ${KLSK_BENCHMARK_OPTS})
  target_link_libraries(bm_klsk_short_code PRIVATE ${KLSK_BENCHMARK_LIBS})

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
