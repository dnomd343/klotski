#include "all_cases.h"
#include "common_code.h"
#include "gtest/gtest.h"
#include "global_utils.h"

using klotski::AllCases;
using klotski::CommonCode;

std::vector<uint64_t> common_code_search(uint64_t start, uint64_t end) {
    std::vector<uint64_t> ret;
    for (uint64_t common_code = start; common_code < end; ++common_code) {
        if (CommonCode::check(common_code)) {
            ret.emplace_back(common_code); // valid common code
        }
    }
    return ret;
}

TEST(GLOBAL, common_code) {
    /// create common code check tasks
    auto pool = TinyPool();
    std::vector<std::future<std::vector<uint64_t>>> futures;
    for (const auto &range : range_split(0, 0x10'0000'0000, 0x10'0000)) {
        futures.emplace_back(
            pool.submit(common_code_search, range.first, range.second)
        );
    }

    /// run common code search
    pool.boot();
    std::vector<uint64_t> result;
    for (auto &f : futures) {
        auto ret = f.get();
        result.insert(result.end(), ret.begin(), ret.end());
    }
    pool.join();

    /// verify check result
    std::vector<uint64_t> all_cases;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            all_cases.emplace_back(head << 32 | range);
        }
    }
    EXPECT_EQ(result, all_cases);
}
