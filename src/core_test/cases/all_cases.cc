#include <string>
#include <thread>
#include <vector>

#include "md5sum.h"
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

TEST(Cases, basic_ranges_build) {
    std::vector<std::thread> threads;
    threads.reserve(TEST_THREAD_NUM);
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        threads.emplace_back([]() {
            BasicRanges::Instance().Build();
        });
    }
    for (auto &t : threads) {
        t.join();
    }
    EXPECT_TRUE(BasicRanges::Instance().IsAvailable());
}

TEST(Cases, basic_ranges_size) {
    auto &basic_ranges = BasicRanges::Instance().Fetch();
    EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_NUM);
}

TEST(Cases, basic_ranges_data) {
    std::string basic_ranges_str;
    basic_ranges_str.reserve(BASIC_RANGES_NUM * 9); // 8-bit + '\n'`
    for (auto range : BasicRanges::Instance().Fetch()) {
        char *tmp = nullptr;
        asprintf(&tmp, "%08X\n", range);
        basic_ranges_str += tmp;
    }
    EXPECT_EQ(md5sum(basic_ranges_str), BASIC_RANGES_MD5);
}

TEST(Cases, all_cases_build) {
    std::vector<std::thread> threads;
    threads.reserve(TEST_THREAD_NUM);
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        threads.emplace_back([]() {
            AllCases::Instance().Build();
        });
    }
    for (auto &t : threads) {
        t.join();
    }
    EXPECT_TRUE(AllCases::Instance().IsAvailable());
}

TEST(Cases, all_cases_size) {
    auto &all_cases = AllCases::Instance().Fetch();
    for (int head = 0; head < 16; ++head) {
        EXPECT_EQ(all_cases[head].size(), ALL_CASES_NUM[head]);
    }
    auto all_cases_num = 0;
    for (auto num : ALL_CASES_NUM) {
        all_cases_num += num;
    }
    EXPECT_EQ(all_cases_num, ALL_CASES_NUM_);
}

TEST(Cases, all_cases_data) {
    std::string all_cases_str;
    all_cases_str.reserve(ALL_CASES_NUM_ * 10); // 9-bit + '\n'
    for (uint64_t head = 0; head < 16; ++head) {
        for (auto range : AllCases::Instance().Fetch()[head]) {
            char *tmp = nullptr;
            asprintf(&tmp, "%09llX\n", head << 32 | range);
            all_cases_str += tmp;
        }
    }
    EXPECT_EQ(md5sum(all_cases_str), ALL_CASES_MD5);
}

// TODO: test all_cases_parallel_build

TEST(Cases, thread_pool_demo) {

    BasicRanges::Instance().Build();

    BS::thread_pool pool;

    std::cout << pool.get_thread_count() << std::endl;

    auto start = clock();
    auto start_ = std::chrono::high_resolution_clock::now();

    AllCases::Instance().BuildParallel([&pool](std::function<void()> &&func) {
//        std::cout << "receive new task" << std::endl;
        pool.push_task(func);
    });

//    std::cout << "parallel build complete" << std::endl;

    pool.wait_for_tasks();

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_);
    std::cerr << elapsed.count() / 1000 / 1000 << "ms" << std::endl;

//    std::cout << "pool tasks complete" << std::endl;

}
