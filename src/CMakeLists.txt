cmake_minimum_required(VERSION 3.0)

include_directories(core)
include_directories(utils)
include_directories(all_cases)

include_directories(raw_code)
include_directories(short_code)
include_directories(common_code)

add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(all_cases)

add_subdirectory(raw_code)
add_subdirectory(short_code)
add_subdirectory(common_code)

#add_executable(klotski main.cc)
add_executable(klotski main.cc fast_cal.cc)

target_link_libraries(klotski core)
target_link_libraries(klotski utils)
target_link_libraries(klotski all_cases)

target_link_libraries(klotski raw_code)
target_link_libraries(klotski short_code)
target_link_libraries(klotski common_code)

target_link_libraries(klotski pthread)
