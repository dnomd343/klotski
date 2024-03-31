set(KLOTSKI_THIRD_PARTY ${KLOTSKI_ROOT_DIR}/third_party)

# abseil library
set(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(${KLOTSKI_THIRD_PARTY}/abseil-cpp EXCLUDE_FROM_ALL)

if (KLOTSKI_ENABLE_BENCHMARK)
  # google benchmark framework
  set(BENCHMARK_ENABLE_TESTING OFF)
  set(BENCHMARK_ENABLE_EXCEPTIONS OFF)
  add_subdirectory(${KLOTSKI_THIRD_PARTY}/benchmark EXCLUDE_FROM_ALL)
endif()

if (KLOTSKI_ENABLE_TESTING)
  # BS thread pool
  add_library(thread_pool INTERFACE)
  target_include_directories(thread_pool INTERFACE
          ${KLOTSKI_THIRD_PARTY}/thread-pool/include)
  add_library(bs::thread_pool ALIAS thread_pool)

  # md5sum implementation
  set(MD5_ENABLE_LTO OFF)
  add_subdirectory(${KLOTSKI_THIRD_PARTY}/md5sum EXCLUDE_FROM_ALL)

  # google test framework
  add_subdirectory(${KLOTSKI_THIRD_PARTY}/googletest EXCLUDE_FROM_ALL)

  # xxHash implementation
  set(BUILD_SHARED_LIBS OFF)
  set(XXHASH_BUILD_XXHSUM OFF)
  set(XXHASH_BUILD_ENABLE_INLINE_API ON)
  add_subdirectory(${KLOTSKI_THIRD_PARTY}/xxHash/cmake_unofficial EXCLUDE_FROM_ALL)
  target_include_directories(xxhash INTERFACE ${KLOTSKI_THIRD_PARTY}/xxHash)
endif()
