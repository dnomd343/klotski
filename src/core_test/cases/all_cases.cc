#include <string>
#include <vector>

#include "xxhsum.h"
#include "exposer.h"
#include "all_cases.h"
#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"

using xxhash::xxhsum;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;

static const auto TEST_THREAD_NUM = 256;

static const std::string ALL_CASES_XXHASH = "d589c8a45983ebb6";
static const std::string BASIC_RANGES_XXHASH = "5e7f633b7bd8af37";

/// Forcibly modify private variables to reset state.
PRIVATE_ACCESS(AllCases, available_, bool)
PRIVATE_ACCESS(BasicRanges, available_, bool)

/// Reset basic ranges build state, note it is thread-unsafe.
void basic_ranges_reset() {
    access_BasicRanges_available_(BasicRanges::instance()) = false;
}

/// Reset all cases build state, note it is thread-unsafe.
void all_cases_reset() {
    access_AllCases_available_(AllCases::instance()) = false;
}

/// Verify that whether basic ranges data is correct.
void basic_ranges_verify() {
    auto &basic_ranges = BasicRanges::instance().fetch();
    EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_NUM); // verify basic ranges size
    EXPECT_EQ(xxhsum(basic_ranges), BASIC_RANGES_XXHASH); // verify basic ranges checksum
}

/// Verify that whether all cases data is correct.
void all_cases_verify() {
    auto &all_cases = AllCases::instance().fetch();
    for (int head = 0; head < 16; ++head) {
        EXPECT_EQ(all_cases[head].size(), ALL_CASES_NUM[head]); // verify all cases size
    }

    auto all_cases_num = 0;
    std::for_each(all_cases.begin(), all_cases.end(), [&all_cases_num](auto &ranges) {
        all_cases_num += ranges.size();
    });
    EXPECT_EQ(all_cases_num, ALL_CASES_NUM_); // verify all cases global size

    std::string all_cases_xxh;
    for (uint64_t head = 0; head < 16; ++head) {
        all_cases_xxh += xxhsum(AllCases::instance().fetch()[head]);
    }
    EXPECT_EQ(xxhsum(all_cases_xxh), ALL_CASES_XXHASH); // verify all cases checksum
}

std::unique_ptr<BS::thread_pool> race_test(int parallel, const std::function<void()> &item) {
    auto pool = std::make_unique<BS::thread_pool>(parallel);
    pool->detach_sequence(0, parallel, [&item](const int) {
        item();
    });
    return pool;
}

TEST(AllCases, basic_ranges) {
    basic_ranges_reset();
    EXPECT_FALSE(BasicRanges::instance().is_available());
    BasicRanges::instance().build();
    EXPECT_TRUE(BasicRanges::instance().is_available());
    BasicRanges::instance().build();
    EXPECT_TRUE(BasicRanges::instance().is_available());
    basic_ranges_verify();
}

TEST(AllCases, basic_ranges_mutex) {
    basic_ranges_reset();
    const auto handle = race_test(TEST_THREAD_NUM, []() {
        BasicRanges::instance().build();
    });
    EXPECT_FALSE(BasicRanges::instance().is_available());
    handle->wait();
    EXPECT_TRUE(BasicRanges::instance().is_available());
    basic_ranges_verify();
}

TEST(AllCases, all_cases) {
    all_cases_reset();
    EXPECT_FALSE(AllCases::instance().is_available());
    AllCases::instance().build();
    EXPECT_TRUE(AllCases::instance().is_available());
    AllCases::instance().build();
    EXPECT_TRUE(AllCases::instance().is_available());
    all_cases_verify();
}

TEST(AllCases, all_cases_mutex) {
    all_cases_reset();
    const auto handle = race_test(TEST_THREAD_NUM, []() {
        AllCases::instance().build();
    });
    EXPECT_FALSE(AllCases::instance().is_available());
    handle->wait();
    EXPECT_TRUE(AllCases::instance().is_available());
    all_cases_verify();
}

TEST(AllCases, all_cases_parallel) {
    all_cases_reset();
    BS::thread_pool executor;
    EXPECT_FALSE(AllCases::instance().is_available());
    AllCases::instance().build_parallel([&executor](auto &&func) {
        executor.detach_task(func);
    });
    EXPECT_TRUE(AllCases::instance().is_available());
    AllCases::instance().build_parallel([&executor](auto &&func) {
        executor.detach_task(func);
    });
    EXPECT_TRUE(AllCases::instance().is_available());
    all_cases_verify();
}

TEST(AllCases, all_cases_parallel_mutex) {
    all_cases_reset();
    BS::thread_pool executor;
    const auto handle = race_test(TEST_THREAD_NUM, [&executor]() {
        AllCases::instance().build_parallel([&executor](auto &&func) {
            executor.detach_task(func);
        });
    });
    EXPECT_FALSE(AllCases::instance().is_available());
    handle->wait();
    EXPECT_TRUE(AllCases::instance().is_available());
    all_cases_verify();
}

TEST(AllCases, all_cases_async) {
    all_cases_reset();
    std::atomic_flag flag;
    BS::thread_pool executor;

    flag.clear();
    AllCases::instance().build_parallel_async([&executor](auto &&func) {
        executor.detach_task(func);
    }, [&flag]() { // callback function
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(AllCases::instance().is_available());
    flag.wait(false);
    EXPECT_TRUE(AllCases::instance().is_available());

    flag.clear();
    AllCases::instance().build_parallel_async([&executor](auto &&func) {
        executor.detach_task(func);
    }, [&flag]() { // callback function
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_TRUE(AllCases::instance().is_available());
    flag.wait(false);
    EXPECT_TRUE(AllCases::instance().is_available());
    all_cases_verify();
}

TEST(AllCases, all_cases_async_mutex) {
    all_cases_reset();
    BS::thread_pool executor;
    std::atomic<int> callback_num(0);

    const auto handle = race_test(TEST_THREAD_NUM, [&executor, &callback_num]() {
        AllCases::instance().build_parallel_async([&executor](auto &&func) {
            executor.detach_task(func);
        }, [&callback_num]() {
            callback_num.fetch_add(1);
        });
    });
    EXPECT_FALSE(AllCases::instance().is_available());
    handle->wait();
    EXPECT_TRUE(AllCases::instance().is_available());
    EXPECT_EQ(callback_num.load(), TEST_THREAD_NUM);
    all_cases_verify();
}
