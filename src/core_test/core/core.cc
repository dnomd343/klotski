#include <gtest/gtest.h>

#include "core/core.h"

#include <unordered_set>

#include "all_cases/all_cases.h"
#include "common_code/common_code.h"

using klotski::core::Core;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;

// mask test

TEST(core, core) {

    std::vector<uint64_t> raw_codes;
    raw_codes.reserve(klotski::cases::ALL_CASES_NUM_);

    // std::unordered_set<uint64_t> codes;
    // codes.reserve(klotski::cases::ALL_CASES_NUM_);

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = common_code.to_raw_code().unwrap();

            raw_codes.emplace_back(raw_code);
            // codes.emplace(raw_code);
        }
    }

    // auto core = Core([&codes](uint64_t ret, uint64_t) {
    //     EXPECT_EQ(codes.count(ret), 1);
    // });

    std::vector<uint64_t> codes;
    codes.reserve(402258220);

    auto core = Core([&codes](uint64_t ret, uint64_t) {
        codes.emplace_back(klotski::codec::RawCode::unsafe_create(ret).to_common_code().unwrap());
    });

    for (auto raw_code : raw_codes) {
        core.next_cases(raw_code, 0);
    }

    // std::cout << codes.size() << std::endl;

}
