#include <vector>

#include "hash.h"
#include "exposer.h"
#include "all_cases.h"
#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;

static constexpr int TEST_THREAD_NUM = 256;

static constexpr uint64_t BASIC_RANGES_XXH3 = 0x82b040060044e336;

static constexpr std::array<uint64_t, 16> ALL_CASES_XXH3 = {
    0x71c8ff7a71c93da0, 0x2a5247ee8bfed666, 0xf4efc8fc692d58e2, 0x2d06800538d394c2,
    0xb3f7cc1b962d6944, 0x7e2792f8ab777faa, 0x4b8e78026cca8a27, 0x2d06800538d394c2,
    0x8acd688c5ab93c42, 0xedca5101ed81cc77, 0xe8dc9d30c91ce682, 0x2d06800538d394c2,
    0x2cdf6c14a7ce3e9a, 0xb9dd04a315583f5c, 0x19046e49c44ae90d, 0x2d06800538d394c2,
};

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
    EXPECT_EQ(hash::xxh3(basic_ranges), BASIC_RANGES_XXH3); // verify basic ranges checksum
}

/// Verify that whether all cases data is correct.
void all_cases_verify() {
    const auto &all_cases = AllCases::instance().fetch();
    for (int head = 0; head < 16; ++head) {
        EXPECT_EQ(all_cases[head].size(), ALL_CASES_NUM[head]); // verify all cases size
    }

    auto all_cases_num = 0;
    std::for_each(all_cases.begin(), all_cases.end(), [&all_cases_num](auto &ranges) {
        all_cases_num += ranges.size();
    });
    EXPECT_EQ(all_cases_num, ALL_CASES_NUM_); // verify all cases global size

    for (uint64_t head = 0; head < 16; ++head) {
        EXPECT_EQ(hash::xxh3(all_cases[head]), ALL_CASES_XXH3[head]); // verify all cases checksum
    }
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
