#include <unordered_set>
#include "core.h"
#include "group.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::GroupType;

using klotski::TYPE_ID_LIMIT;

TEST(Core, next_cases) {
    auto test = [](uint32_t type_id) {
        auto raw_codes = std::unordered_set<uint64_t>();
        raw_codes.reserve(klotski::ALL_CASES_SIZE_SUM);
        for (auto &&common_code: GroupType(type_id).cases()) { // load all cases in a type
            raw_codes.emplace(common_code.to_raw_code().unwrap());
        }

        auto core = klotski::Core(
            [&raw_codes](auto &&code, auto &&mask) {
                EXPECT_NE(raw_codes.find(code), raw_codes.end()); // verify next case
                EXPECT_EQ(mask >> __builtin_ctzll(mask), 0b111); // verify mask
            }
        );
        for (auto &&raw_code: raw_codes) {
            core.next_cases(raw_code, 0); // search next cases
        }
    };
    // TODO: using multi-threads
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        test(type_id);
    }
}
