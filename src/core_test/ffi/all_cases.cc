#include <future>
#include <string>
#include <thread>

#include "md5.h"
#include "exposer.h"
#include "klotski.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using md5::MD5;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;

/// The efficiency of string hashing is not very high, but there is a memorable
/// story, and this scheme is still retained here.
static const std::string ALL_CASES_MD5 = "3888e9fab8d3cbb50908b12b147cfb23";

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

    flag.clear();
    all_cases_prebuild_async([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    }, []() { // callback function
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(all_cases_prebuild_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_prebuild_available());

    flag.clear();
    all_cases_prebuild_async([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    }, []() { // callback function
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

    flag.clear();
    all_cases_build_async([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    }, []() { // callback function
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(all_cases_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_available());

    flag.clear();
    all_cases_build_async([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    }, []() { // callback function
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_TRUE(all_cases_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_available());
}

TEST(AllCases, all_cases_build_parallel) {
    all_cases_reset();
    EXPECT_FALSE(all_cases_available());
    all_cases_build_parallel([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    });
    EXPECT_TRUE(all_cases_available());
    all_cases_build_parallel([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    });
    EXPECT_TRUE(all_cases_available());
}

TEST(AllCases, all_cases_build_parallel_async) {
    all_cases_reset();
    static std::atomic_flag flag;

    flag.clear();
    all_cases_build_parallel_async([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    }, []() { // callback function
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(all_cases_available());
    flag.wait(false);
    EXPECT_TRUE(all_cases_available());

    flag.clear();
    all_cases_build_parallel_async([](void (*fn)(void*), void *arg) {
        std::thread worker(fn, arg);
        worker.detach();
    }, []() { // callback function
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
        auto num = all_cases_num((int)head);
        auto *ranges = all_cases_export((int)head);
        for (auto i = 0; i < num; ++i) {
            char *tmp;
            asprintf(&tmp, "%09llX\n", head << 32 | ranges[i]);
            all_cases_str += tmp;
        }
    }
    EXPECT_EQ(MD5::Hash(all_cases_str), ALL_CASES_MD5);
}
