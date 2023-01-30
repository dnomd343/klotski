#include "klotski.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::AllCases;
using klotski::BasicRanges;

TEST(FFI, all_cases_build) {
    // basic ranges test
    EXPECT_EQ(is_basic_ranges_available(), BasicRanges::status() == BasicRanges::AVAILABLE);
    basic_ranges_build();
    EXPECT_EQ(is_basic_ranges_available(), true);
    EXPECT_EQ(is_basic_ranges_available(), BasicRanges::status() == BasicRanges::AVAILABLE);

    // all cases test
    EXPECT_EQ(is_all_cases_available(), AllCases::status() == AllCases::AVAILABLE);
    all_cases_build();
    EXPECT_EQ(is_all_cases_available(), true);
    EXPECT_EQ(is_all_cases_available(), AllCases::status() == AllCases::AVAILABLE);
}

TEST(FFI, all_cases_size) {
    EXPECT_EQ(ALL_CASES_SIZE, klotski::ALL_CASES_SIZE_SUM);
    EXPECT_EQ(BASIC_RANGES_SIZE, klotski::BASIC_RANGES_SIZE);
}

TEST(FFI, all_cases_data) {
    auto *basic_ranges_buf = new uint32_t[BASIC_RANGES_SIZE];
    export_basic_ranges(basic_ranges_buf);
    for (uint32_t i = 0; i < BASIC_RANGES_SIZE; ++i) { // basic ranges data verify
        EXPECT_EQ(basic_ranges_buf[i], BasicRanges::fetch()[i]);
    }
    delete []basic_ranges_buf;

    auto *all_cases_buf = new uint64_t[ALL_CASES_SIZE];
    export_all_cases(all_cases_buf);
    uint64_t index = 0;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) { // all cases data verify
            EXPECT_EQ(all_cases_buf[index++], head << 32 | range);
        }
    }
    delete []all_cases_buf;
}
