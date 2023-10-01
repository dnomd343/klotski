set(KLOTSKI_THIRD_PARTY ${KLOTSKI_ROOT}/third_party)

set(ABSL_PROPAGATE_CXX_STD ON)
include_directories(abseil-cpp)
add_subdirectory(${KLOTSKI_THIRD_PARTY}/abseil-cpp EXCLUDE_FROM_ALL)

if (KLOTSKI_UNIT_TEST)
    include_directories(${KLOTSKI_THIRD_PARTY}/md5sum)
    add_subdirectory(${KLOTSKI_THIRD_PARTY}/md5sum EXCLUDE_FROM_ALL)
endif (KLOTSKI_UNIT_TEST)

if (KLOTSKI_UNIT_TEST)
    include_directories(${KLOTSKI_THIRD_PARTY}/googletest/googletest/include)
    add_subdirectory(${KLOTSKI_THIRD_PARTY}/googletest EXCLUDE_FROM_ALL)
endif (KLOTSKI_UNIT_TEST)

if (KLOTSKI_UNIT_TEST)
    include_directories(${KLOTSKI_THIRD_PARTY}/thread-pool/include)
endif (KLOTSKI_UNIT_TEST)
