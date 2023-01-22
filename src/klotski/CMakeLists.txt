cmake_minimum_required(VERSION 3.0)

################################################

add_subdirectory(utils)
add_subdirectory(all_cases)

add_subdirectory(raw_code)
add_subdirectory(short_code)
add_subdirectory(common_code)

add_subdirectory(core)
add_subdirectory(analyse)
add_subdirectory(fast_cal)

#add_subdirectory(graph)
#add_subdirectory(benchmark)

################################################

#add_library(klotski klotski_ffi.cc)
#add_library(klotski STATIC klotski_ffi.cc)
add_library(klotski SHARED klotski_ffi.cc)

target_link_libraries(klotski utils)
target_link_libraries(klotski all_cases)

target_link_libraries(klotski raw_code)
target_link_libraries(klotski short_code)
target_link_libraries(klotski common_code)

target_link_libraries(klotski core)
target_link_libraries(klotski analyse)
target_link_libraries(klotski fast_cal)

################################################

#target_link_libraries(klotski graph)
#target_link_libraries(klotski benchmark)
#target_link_libraries(klotski pthread)
