#include <thread>
#include <cstdint>
#include "md5.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::AllCases;
using klotski::BasicRanges;
using klotski::ALL_CASES_SIZE;
using klotski::BASIC_RANGES_SIZE;
using klotski::ALL_CASES_SIZE_SUM;

const static int TEST_THREAD_NUM = 8;

/// basic ranges constants
const char BASIC_RANGES_MD5[] = "6f385dc171e201089ff96bb010b47212";

/// all cases constants
const char ALL_CASES_MD5[] = "3888e9fab8d3cbb50908b12b147cfb23";

/// basic ranges mutex check
TEST(AllCases, basic_ranges_mutex) {
    std::thread threads[TEST_THREAD_NUM];
    EXPECT_EQ(BasicRanges::status(), BasicRanges::NO_INIT);
    for (auto &t : threads) {
        t = std::thread(BasicRanges::build);
    }
    EXPECT_EQ(BasicRanges::status(), BasicRanges::BUILDING);
    for (auto &t : threads) {
        t.join();
    }
    EXPECT_EQ(BasicRanges::status(), BasicRanges::AVAILABLE);
}

/// basic ranges size check
TEST(AllCases, basic_ranges_size) {
    auto &basic_ranges = BasicRanges::fetch();
    EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_SIZE);
}

/// basic ranges data verify
TEST(AllCases, basic_ranges_data) {
    auto *basic_ranges_data = new char[BASIC_RANGES_SIZE * 9];
    auto *current = basic_ranges_data;
    for (const auto &range : BasicRanges::fetch()) {
        sprintf(current, "%08X\n", range);
        current += 9;
    }
    auto basic_ranges_md5 = md5(basic_ranges_data, BASIC_RANGES_SIZE * 9);
    EXPECT_STREQ(basic_ranges_md5.c_str(), BASIC_RANGES_MD5);
}

/// basic ranges mutex check
TEST(AllCases, all_cases_mutex) {
    std::thread threads[TEST_THREAD_NUM];
    EXPECT_EQ(AllCases::status(), AllCases::NO_INIT);
    for (auto &t : threads) {
        t = std::thread(AllCases::build);
    }
    EXPECT_EQ(AllCases::status(), AllCases::BUILDING);
    for (auto &t : threads) {
        t.join();
    }
    EXPECT_EQ(AllCases::status(), AllCases::AVAILABLE);
}

/// all cases size check
TEST(AllCases, all_cases_size) {
    auto &all_cases = AllCases::fetch();
    for (uint32_t head = 0; head < 16; ++head) {
        EXPECT_EQ(all_cases[head].size(), ALL_CASES_SIZE[head]);
    }
}

/// all cases data verify
TEST(AllCases, all_cases_data) {
    auto *all_cases_data = new char[ALL_CASES_SIZE_SUM * 10];
    auto *current = all_cases_data;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            sprintf(current, "%09lX\n", head << 32 | range);
            current += 10;
        }
    }
    auto all_cases_md5 = md5(all_cases_data, ALL_CASES_SIZE_SUM * 10);
    EXPECT_STREQ(all_cases_md5.c_str(), ALL_CASES_MD5);
}
