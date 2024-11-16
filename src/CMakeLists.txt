cmake_minimum_required(VERSION 3.12)

add_subdirectory(core)
add_subdirectory(core_ffi)

if (KLSK_ENABLE_TESTING)
  add_subdirectory(core_test)
endif()

#project(klotski-cli LANGUAGES C)
#set(CMAKE_C_STANDARD 90)
