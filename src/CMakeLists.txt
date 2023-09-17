cmake_minimum_required(VERSION 3.0)

project(klotski-cli LANGUAGES C)

add_subdirectory(core)

#add_subdirectory(klotski_core)
#include_directories(klotski_core)

#add_executable(cli main.c)
#target_link_libraries(cli PRIVATE klotski absl::flat_hash_map)

# -labsl_hash -labsl_city -labsl_low_level_hash -labsl_raw_hash_set
