#include <string>
#include <vector>

#include "md5sum.h"
#include "exposer.h"
#include "all_cases.h"
#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"

using md5::md5sum;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;

static const auto TEST_THREAD_NUM = 256;

/// The efficiency of string hashing is not very high, but there is a memorable
/// story, and this scheme is still retained here.
static const std::string ALL_CASES_MD5 = "3888e9fab8d3cbb50908b12b147cfb23";
static const std::string BASIC_RANGES_MD5 = "6f385dc171e201089ff96bb010b47212";

/// Forcibly modify private variables to reset state.
PRIVATE_ACCESS(AllCases, available_, bool)
PRIVATE_ACCESS(BasicRanges, available_, bool)

/// Reset basic ranges build state, note it is thread-unsafe.
void basic_ranges_reset() {
    access_BasicRanges_available_(BasicRanges::Instance()) = false;
}

/// Reset all cases build state, note it is thread-unsafe.
void all_cases_reset() {
    access_AllCases_available_(AllCases::Instance()) = false;
}

/// Verify that whether basic ranges data is correct.
void basic_ranges_verify() {
    auto &basic_ranges = BasicRanges::Instance().Fetch();
    EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_NUM); // verify basic ranges size

    std::string basic_ranges_str;
    basic_ranges_str.reserve(BASIC_RANGES_NUM * 9); // 8-bit + '\n'`
    for (auto range : basic_ranges) {
        char *tmp = nullptr;
        asprintf(&tmp, "%08X\n", range);
        basic_ranges_str += tmp;
    }
    EXPECT_EQ(md5sum(basic_ranges_str), BASIC_RANGES_MD5); // verify basic ranges checksum
}

/// Verify that whether all cases data is correct.
void all_cases_verify() {
    auto &all_cases = AllCases::Instance().Fetch();
    for (int head = 0; head < 16; ++head) {
        EXPECT_EQ(all_cases[head].size(), ALL_CASES_NUM[head]); // verify all cases size
    }
    auto all_cases_num = 0;
    std::for_each(all_cases.begin(), all_cases.end(), [&all_cases_num](auto &ranges) {
        all_cases_num += ranges.size();
    });
    EXPECT_EQ(all_cases_num, ALL_CASES_NUM_); // verify all cases global size

    std::string all_cases_str;
    all_cases_str.reserve(ALL_CASES_NUM_ * 10); // 9-bit + '\n'
    for (uint64_t head = 0; head < 16; ++head) {
        for (auto range : AllCases::Instance().Fetch()[head]) {
            char *tmp = nullptr;
            asprintf(&tmp, "%09llX\n", head << 32 | range);
            all_cases_str += tmp;
        }
    }
    EXPECT_EQ(md5sum(all_cases_str), ALL_CASES_MD5); // verify all cases checksum
}

TEST(Cases, basic_ranges) {
    basic_ranges_reset();
    EXPECT_FALSE(BasicRanges::Instance().IsAvailable());
    BasicRanges::Instance().Build();
    EXPECT_TRUE(BasicRanges::Instance().IsAvailable());
    BasicRanges::Instance().Build();
    EXPECT_TRUE(BasicRanges::Instance().IsAvailable());
    basic_ranges_verify();
}

TEST(Cases, basic_ranges_mutex) {
    basic_ranges_reset();
    BS::thread_pool pool(TEST_THREAD_NUM);

    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        auto _ = pool.submit(&BasicRanges::Build, &BasicRanges::Instance());
    }
    EXPECT_FALSE(BasicRanges::Instance().IsAvailable());
    pool.wait_for_tasks();
    EXPECT_TRUE(BasicRanges::Instance().IsAvailable());
    basic_ranges_verify();
}

TEST(Cases, all_cases) {
    all_cases_reset();
    EXPECT_FALSE(AllCases::Instance().IsAvailable());
    AllCases::Instance().Build();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    AllCases::Instance().Build();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    all_cases_verify();
}

TEST(Cases, all_cases_mutex) {
    all_cases_reset();
    BS::thread_pool pool(TEST_THREAD_NUM);

    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        auto _ = pool.submit(&AllCases::Build, &AllCases::Instance());
    }
    EXPECT_FALSE(AllCases::Instance().IsAvailable());
    pool.wait_for_tasks();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    all_cases_verify();
}

TEST(Cases, all_cases_parallel) {
    all_cases_reset();
    BS::thread_pool executor;
    EXPECT_FALSE(AllCases::Instance().IsAvailable());
    AllCases::Instance().BuildParallel([&executor](auto &&func) {
        executor.push_task(func);
    });
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    AllCases::Instance().BuildParallel([&executor](auto &&func) {
        executor.push_task(func);
    });
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    all_cases_verify();
}

TEST(Cases, all_cases_parallel_mutex) {
    all_cases_reset();
    BS::thread_pool executor;
    BS::thread_pool pool(TEST_THREAD_NUM);

    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        auto _ = pool.submit(&AllCases::BuildParallel, &AllCases::Instance(), [&executor](auto &&func) {
            executor.push_task(func);
        });
    }
    EXPECT_FALSE(AllCases::Instance().IsAvailable());
    pool.wait_for_tasks();
    executor.wait_for_tasks();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    all_cases_verify();
}

TEST(Cases, all_cases_async) {
    all_cases_reset();
    BS::thread_pool executor;

    std::promise<void> promise_1;
    auto future_1 = promise_1.get_future();
    AllCases::Instance().BuildParallelAsync([&executor](auto &&func) {
        executor.push_task(func);
    }, [&promise_1]() {
        promise_1.set_value();
    });
    EXPECT_FALSE(AllCases::Instance().IsAvailable());
    future_1.wait();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());

    std::promise<void> promise_2;
    auto future_2 = promise_2.get_future();
    AllCases::Instance().BuildParallelAsync([&executor](auto &&func) {
        executor.push_task(func);
    }, [&promise_2]() {
        promise_2.set_value();
    });
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    future_2.wait();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    all_cases_verify();
}

TEST(Cases, all_cases_async_mutex) {
    all_cases_reset();
    BS::thread_pool executor;
    std::atomic<int> callback_num(0);
    BS::thread_pool pool(TEST_THREAD_NUM);

    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        auto _ = pool.submit(&AllCases::BuildParallelAsync, &AllCases::Instance(), [&executor](auto &&func) {
            executor.push_task(func);
        }, [&callback_num]() {
            callback_num.fetch_add(1);
        });
    }
    EXPECT_FALSE(AllCases::Instance().IsAvailable());
    pool.wait_for_tasks();
    executor.wait_for_tasks();
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
    EXPECT_EQ(callback_num.load(), TEST_THREAD_NUM);
    all_cases_verify();
}
