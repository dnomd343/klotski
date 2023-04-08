#include <unordered_set>
#include "core.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::AllCases;

TEST(Core, demo) {
    auto raw_codes = std::unordered_set<uint64_t>();
//    auto raw_codes_ = std::vector<uint64_t>();

    raw_codes.reserve(klotski::ALL_CASES_SIZE_SUM);
//    raw_codes_.reserve(klotski::ALL_CASES_SIZE_SUM);

    for (auto &&common_code : AllCases::release()) { // load all raw codes

//        EXPECT_NE(common_code.to_raw_code(), 0);

        raw_codes.emplace(common_code.to_raw_code().unwrap());
//        raw_codes_.emplace_back(common_code.to_raw_code().unwrap());
    }

    std::cout << "init ok" << std::endl;

    auto core = klotski::Core(
        [&raw_codes](auto &&code, auto &&mask) {
            EXPECT_NE(raw_codes.find(code), raw_codes.end());
//            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );


    for (auto &&raw_code : raw_codes) {
        core.next_cases(raw_code, 0);
    }


//    EXPECT_EQ(raw_codes.size(), 29334498);
}
