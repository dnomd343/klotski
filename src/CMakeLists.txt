cmake_minimum_required(VERSION 3.0)

project(klotski-cli LANGUAGES C)

add_subdirectory(core)
add_subdirectory(core_test)

#add_subdirectory(klotski_core)
#include_directories(klotski_core)

set(CMAKE_C_STANDARD 90)

add_executable(klotski_cli main.c)
target_link_libraries(klotski_cli PRIVATE klotski-core)
#target_link_libraries(klotski_cli PRIVATE klotski absl::flat_hash_map)

# -labsl_hash -labsl_city -labsl_low_level_hash -labsl_raw_hash_set
