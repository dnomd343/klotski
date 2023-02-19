#include "all_cases.h"
#include "short_code.h"
#include "gtest/gtest.h"
#include "global_utils.h"

using klotski::AllCases;
using klotski::ShortCode;
using klotski::CommonCode;

std::vector<uint64_t> short_code_check(uint32_t start, uint32_t end) {
    std::vector<uint64_t> archive;
    for (uint32_t short_code = start; short_code < end; ++short_code) {
        auto common_code = ShortCode::unsafe_create(short_code).to_common_code();
        archive.emplace_back(common_code.unwrap());
        EXPECT_EQ(common_code.to_short_code().unwrap(), short_code);
    }
    return archive;
}

TEST(GLOBAL, short_code) {
    /// create short code check tasks
    auto pool = TinyPool(thread_num());
    ShortCode::speed_up(ShortCode::NORMAL);
    std::vector<std::future<std::vector<uint64_t>>> futures;

    for (const auto &range : range_split(0, klotski::ALL_CASES_SIZE_SUM, 10000)) {
//    for (const auto &range : range_split(0, 80000, 10000)) {

//        std::cout << "[" << range.first << ", " << range.second << ")" << std::endl;

        futures.emplace_back(
            pool.submit(short_code_check, range.first, range.second)
        );
    }

    /// run short code search
    pool.boot();
    std::vector<uint64_t> result;
    for (auto &f : futures) {
        auto ret = f.get();
        result.insert(result.end(), ret.begin(), ret.end());
    }
    pool.join();

//    std::cout << result.size() << std::endl;

    /// verify check result
    std::vector<uint64_t> all_cases;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            all_cases.emplace_back(head << 32 | range);
        }
    }
    EXPECT_EQ(result, all_cases);

}
