set(KLSK_THIRD_PARTY ${KLSK_ROOT_DIR}/third_party)

# parallel hashmap
add_subdirectory(${KLSK_THIRD_PARTY}/parallel-hashmap EXCLUDE_FROM_ALL)
add_library(phmap::phmap ALIAS phmap)

if (KLSK_ENABLE_BENCHMARK)
  # google benchmark framework
  set(BENCHMARK_ENABLE_TESTING OFF)
  set(BENCHMARK_ENABLE_EXCEPTIONS OFF)
  add_subdirectory(${KLSK_THIRD_PARTY}/benchmark EXCLUDE_FROM_ALL)

  # TODO: enable bs::thread_pool
endif()

if (KLSK_ENABLE_TESTING)
  # BS thread pool
  add_library(thread_pool INTERFACE)
  target_include_directories(thread_pool INTERFACE
          ${KLSK_THIRD_PARTY}/thread-pool/include)
  add_library(bs::thread_pool ALIAS thread_pool)

  # md5sum implementation
  set(MD5_ENABLE_LTO OFF)
  add_subdirectory(${KLSK_THIRD_PARTY}/md5sum EXCLUDE_FROM_ALL)

  # google test framework
  add_subdirectory(${KLSK_THIRD_PARTY}/googletest EXCLUDE_FROM_ALL)

  # xxHash implementation
  add_library(xxhash INTERFACE)
  target_include_directories(xxhash INTERFACE ${KLSK_THIRD_PARTY}/xxHash)
  add_library(xxHash::xxh3 ALIAS xxhash)
endif()

if (KLSK_PYTHON_FFI)
  # python ffi exposure
  add_subdirectory(${KLSK_THIRD_PARTY}/pybind11 EXCLUDE_FROM_ALL)
endif()
