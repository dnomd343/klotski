#include "core.h"
#include "group.h"
#include "all_cases.h"
#include "tiny_pool.h"
#include "gtest/gtest.h"
#include "absl/container/flat_hash_set.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::GroupType;

using klotski::TYPE_ID_LIMIT;

TEST(Core, next_cases) {
    auto test = [](GroupType type_id) {
        auto raw_codes = absl::flat_hash_set<uint64_t>();
        raw_codes.reserve(klotski::ALL_CASES_SIZE_SUM);
        for (auto &&common_code: type_id.cases()) { // load all cases in a type
            raw_codes.emplace(common_code.to_raw_code().unwrap());
        }

        auto core = klotski::Core(
            [&raw_codes](auto &&code, auto &&mask) {
                EXPECT_NE(raw_codes.find(code), raw_codes.end()); // next case existed
                EXPECT_EQ(mask >> __builtin_ctzll(mask), 0b111); // mask format
            }
        );
        for (auto &&raw_code: raw_codes) {
            core.next_cases(raw_code, 0); // search next cases
        }
    };

    auto pool = TinyPool();
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        pool.submit(test, GroupType(type_id)); // test all type_ids
    }
    pool.boot().join();
}
