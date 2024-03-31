#include <algorithm>

#include "sample.h"
#include "raw_code.h"
#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"
#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

TEST(CommonCode, validity) {
    EXPECT_NE(CommonCode::check(0x3'A9'BF'0C'00), true); // invalid 2x2 block
    EXPECT_NE(CommonCode::check(0x1'D9'BF'0C'00), true); // invalid block range
    EXPECT_NE(CommonCode::check(0x1'A9'BF'FC'00), true); // less than 2 space
    EXPECT_NE(CommonCode::check(0x1'A0'BF'0C'01), true); // low bits not fill zero

    EXPECT_FALSE(CommonCode::create(0x123456789).has_value()); // invalid code
    EXPECT_FALSE(CommonCode::from_string("0123456789").has_value()); // length > 9
    EXPECT_FALSE(CommonCode::from_string("123J432A9").has_value()); // with invalid `J`
}

TEST(CommonCode, operators) {
    auto common_code = CommonCode::unsafe_create(TEST_C_CODE);

    std::ostringstream tmp;
    tmp << common_code; // ostream capture
    EXPECT_EQ(tmp.str(), TEST_C_CODE_STR);
    EXPECT_EQ((uint64_t)common_code, TEST_C_CODE); // convert as uint64_t

    EXPECT_EQ(TEST_C_CODE, common_code); // uint64_t == CommonCode
    EXPECT_EQ(common_code, TEST_C_CODE); // CommonCode == uint64_t
    EXPECT_EQ(common_code, common_code); // CommonCode == CommonCode

    EXPECT_NE(TEST_C_CODE + 1, common_code); // uint64_t != CommonCode
    EXPECT_NE(common_code, TEST_C_CODE + 1); // CommonCode != uint64_t
    EXPECT_NE(common_code, CommonCode::unsafe_create(TEST_C_CODE + 1)); // CommonCode != CommonCode

    EXPECT_LT(common_code, CommonCode::unsafe_create(TEST_C_CODE + 1)); // CommonCode < CommonCode
    EXPECT_GT(CommonCode::unsafe_create(TEST_C_CODE + 1), common_code); // CommonCode > CommonCode
}

TEST(CommonCode, exporter) {
    auto common_code = CommonCode::unsafe_create(TEST_C_CODE);
    EXPECT_EQ(common_code.unwrap(), TEST_C_CODE);
    EXPECT_EQ(common_code.to_string(), TEST_C_CODE_STR);
    EXPECT_EQ(common_code.to_raw_code(), TEST_R_CODE);
    EXPECT_EQ(common_code.to_short_code(), TEST_S_CODE);

    auto code_shorten = common_code.to_string(true);
    EXPECT_EQ(CommonCode::from_string(code_shorten), common_code); // l-value
    EXPECT_EQ(CommonCode::from_string(std::move(code_shorten)), common_code); // r-value

    auto code_normal = common_code.to_string(false);
    EXPECT_EQ(CommonCode::from_string(code_normal), common_code); // l-value
    EXPECT_EQ(CommonCode::from_string(std::move(code_normal)), common_code); // r-value
}

TEST(CommonCode, initializate) {
    auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);
    auto common_code = CommonCode::unsafe_create(TEST_C_CODE);

    // CommonCode(...)
    EXPECT_EQ(CommonCode(raw_code), TEST_C_CODE);
    EXPECT_EQ(CommonCode(short_code), TEST_C_CODE);
    EXPECT_EQ(CommonCode(common_code), TEST_C_CODE); // l-value
    EXPECT_EQ(CommonCode(CommonCode(common_code)), TEST_C_CODE); // r-value

    // CommonCode::create(uint64_t)
    EXPECT_TRUE(CommonCode::create(TEST_C_CODE).has_value());
    EXPECT_FALSE(CommonCode::create(TEST_C_CODE_ERR).has_value());
    EXPECT_EQ(CommonCode::create(TEST_C_CODE), TEST_C_CODE);

    // CommonCode::unsafe_create(uint64_t)
    EXPECT_EQ(CommonCode::unsafe_create(TEST_C_CODE), TEST_C_CODE);

    // CommonCode::from_string(const std::string &)
    EXPECT_TRUE(CommonCode::from_string(TEST_C_CODE_STR).has_value());
    EXPECT_FALSE(CommonCode::from_string(TEST_C_CODE_STR_ERR).has_value());
    EXPECT_EQ(CommonCode::from_string(TEST_C_CODE_STR), TEST_C_CODE);

    // CommonCode::from_string(std::string &&)
    EXPECT_TRUE(CommonCode::from_string(TEST_C_CODE_STR_RV).has_value());
    EXPECT_FALSE(CommonCode::from_string(TEST_C_CODE_STR_ERR_RV).has_value());
    EXPECT_EQ(CommonCode::from_string(TEST_C_CODE_STR_RV), TEST_C_CODE);

    // CommonCode::from_raw_code(RawCode)
    EXPECT_EQ(CommonCode::from_raw_code(raw_code), TEST_C_CODE);

    // CommonCode::from_raw_code(uint64_t)
    EXPECT_TRUE(CommonCode::from_raw_code(TEST_R_CODE).has_value());
    EXPECT_FALSE(CommonCode::from_raw_code(TEST_R_CODE_ERR).has_value());
    EXPECT_EQ(CommonCode::from_raw_code(TEST_R_CODE), TEST_C_CODE);

    // CommonCode::from_short_code(ShortCode)
    EXPECT_EQ(CommonCode::from_short_code(short_code), TEST_C_CODE);

    // CommonCode::from_short_code(uint32_t)
    EXPECT_TRUE(CommonCode::from_short_code(TEST_S_CODE).has_value());
    EXPECT_FALSE(CommonCode::from_short_code(TEST_S_CODE_ERR).has_value());
    EXPECT_EQ(CommonCode::from_short_code(TEST_S_CODE), TEST_C_CODE);

    // CommonCode::from_short_code(const std::string &)
    EXPECT_TRUE(CommonCode::from_short_code(TEST_S_CODE_STR).has_value());
    EXPECT_FALSE(CommonCode::from_short_code(TEST_S_CODE_STR_ERR).has_value());
    EXPECT_EQ(CommonCode::from_short_code(TEST_S_CODE_STR), TEST_C_CODE);

    // CommonCode::from_short_code(std::string &&)
    EXPECT_TRUE(CommonCode::from_short_code(TEST_S_CODE_STR_RV).has_value());
    EXPECT_FALSE(CommonCode::from_short_code(TEST_S_CODE_STR_ERR_RV).has_value());
    EXPECT_EQ(CommonCode::from_short_code(TEST_S_CODE_STR_RV), TEST_C_CODE);
}

TEST(CommonCode, code_verify) {
    BS::thread_pool pool;
    pool.detach_sequence(0, 16, [](const uint64_t head) {
        for (auto range : AllCases::instance().fetch()[head]) {
            auto code = head << 32 | range;
            EXPECT_TRUE(CommonCode::check(code)); // verify all cases
        }
    });
    pool.wait();
}

TEST(CommonCode, code_string) {
    auto test_func = [](CommonCode code) {
        auto code_shorten = code.to_string(true); // with shorten
        auto code_normal = code.to_string(false); // without shorten
        EXPECT_TRUE(code_normal.starts_with(code_shorten));

        EXPECT_LE(code_shorten.size(), 9); // length -> (0, 9]
        EXPECT_NE(code_shorten.size(), 0);
        if (code != 0) { // skip special code string `0`
            EXPECT_NE(code_shorten.back(), '0');
        }
        EXPECT_EQ(CommonCode::from_string(code_shorten), code); // test upper cases
        std::transform(code_shorten.begin(), code_shorten.end(), code_shorten.begin(), ::tolower);
        EXPECT_EQ(CommonCode::from_string(code_shorten), code); // test lower cases

        EXPECT_EQ(code_normal.size(), 9); // length = 9
        for (auto c : code_normal) {
            EXPECT_TRUE((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
        }
        EXPECT_EQ(CommonCode::from_string(code_normal), code); // test upper cases
        std::transform(code_normal.begin(), code_normal.end(), code_normal.begin(), ::tolower);
        EXPECT_EQ(CommonCode::from_string(code_normal), code); // test lower cases
    };

    BS::thread_pool pool;
    pool.detach_sequence(0, 16, [&test_func](const uint64_t head) {
        for (auto range : AllCases::instance().fetch()[head]) {
            test_func(CommonCode::unsafe_create(head << 32 | range));
        }
    });
    pool.wait();
}

TEST(CommonCode, DISABLED_global_verify) {
    BS::thread_pool pool;
    auto futures = pool.submit_blocks(0ULL, 0x10'0000'0000ULL, [](uint64_t start, uint64_t end) {
        std::vector<uint64_t> archive;
        for (uint64_t common_code = start; common_code < end; ++common_code) { // brute-force search
            if (CommonCode::check(common_code)) {
                archive.emplace_back(common_code); // found valid common code
            }
        }
        return archive;
    }, 0x1000); // split as 4096 pieces

    std::vector<uint64_t> result;
    result.reserve(ALL_CASES_NUM_);
    for (auto &future : futures) {
        const auto data = future.get();
        result.insert(result.end(), data.begin(), data.end()); // combine sections
    }
    EXPECT_EQ(result, all_common_codes());
}
