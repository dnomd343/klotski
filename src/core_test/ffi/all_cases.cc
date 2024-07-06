#include <format>
#include <thread>
#include <gtest/gtest.h>

#include "hash.h"
#include "exposer.h"
#include "klotski.h"
#include "all_cases/all_cases.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::ALL_CASES_NUM;

/// The efficiency of string hashing is not very high, but there is a memorable
/// story, and this scheme is still retained here.
static constexpr std::string_view ALL_CASES_MD5 = "3888e9fab8d3cbb50908b12b147cfb23";

/// Forcibly modify private variables to reset state.
FORCE_ACCESS_VAR(AllCases, bool, available_)
FORCE_ACCESS_VAR(BasicRanges, bool, available_)

/// Reset basic ranges build state, note it is thread-unsafe.
void basic_ranges_reset() {
    exposer::BasicRanges_available_(BasicRanges::instance()) = false;
}

/// Reset all cases build state, note it is thread-unsafe.
void all_cases_reset() {
    exposer::AllCases_available_(AllCases::instance()) = false;
}

TEST(AllCases, all_cases_prebuild) {
    basic_ranges_reset();
    EXPECT_FALSE(all_cases_prebuild_available());
    all_cases_prebuild();
    EXPECT_TRUE(all_cases_prebuild_available());
    all_cases_prebuild();
    EXPECT_TRUE(all_cases_prebuild_available());
}

TEST(AllCases, all_cases_prebuild_async) {
    basic_ranges_reset();

    static std::atomic_flag flag;
    auto executor = [](void (*fn)(void*), void *arg) {
        std::thread worker {fn, arg};
        worker.detach();
    };

    flag.clear();
    all_cases_prebuild_async(executor, [] {
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(all_cases_prebuild_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_prebuild_available());

    flag.clear();
    all_cases_prebuild_async(executor, [] {
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_TRUE(all_cases_prebuild_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_prebuild_available());
}

TEST(AllCases, all_cases_build) {
    all_cases_reset();
    EXPECT_FALSE(all_cases_available());
    all_cases_build();
    EXPECT_TRUE(all_cases_available());
    all_cases_build();
    EXPECT_TRUE(all_cases_available());
}

TEST(AllCases, all_cases_build_async) {
    all_cases_reset();

    static std::atomic_flag flag;
    auto executor = [](void (*fn)(void*), void *arg) {
        std::thread worker {fn, arg};
        worker.detach();
    };

    flag.clear();
    all_cases_build_async(executor, [] {
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(all_cases_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_available());

    flag.clear();
    all_cases_build_async(executor, [] {
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_TRUE(all_cases_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_available());
}

TEST(AllCases, all_cases_num) {
    EXPECT_LT(all_cases_num(-2), 0);
    EXPECT_LT(all_cases_num(-1), 0);
    for (int i = 0; i < 15; ++i) {
        EXPECT_EQ(all_cases_num(i), ALL_CASES_NUM[i]);
    }
    EXPECT_LT(all_cases_num(16), 0);
    EXPECT_LT(all_cases_num(17), 0);
}

TEST(AllCases, all_cases_export) {
    EXPECT_EQ(all_cases_export(-2), nullptr);
    EXPECT_EQ(all_cases_export(-1), nullptr);
    EXPECT_EQ(all_cases_export(16), nullptr);
    EXPECT_EQ(all_cases_export(17), nullptr);

    std::string all_cases_str;
    for (uint64_t head = 0; head < 15; ++head) {
        const auto num = all_cases_num(static_cast<int>(head));
        auto *ranges = all_cases_export(static_cast<int>(head));
        for (auto i = 0; i < num; ++i) {
            all_cases_str += std::format("{:09X}\n", head << 32 | ranges[i]);
        }
    }
    EXPECT_EQ(hash::md5(all_cases_str), ALL_CASES_MD5);
}
