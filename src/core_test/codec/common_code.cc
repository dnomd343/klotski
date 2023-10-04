#include <algorithm>

#include "all_cases.h"
#include "raw_code.h"
#include "short_code.h"
#include "common_code.h"
#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

const static uint64_t TEST_CODE = 0x1'A9BF'0C00;
const static std::string TEST_CODE_STR = "1A9BF0C00";

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
    auto common_code = CommonCode::create(TEST_CODE).value();

    std::ostringstream tmp;
    tmp << common_code; // ostream capture
    EXPECT_EQ(tmp.str(), TEST_CODE_STR);
    EXPECT_EQ((uint64_t)common_code, TEST_CODE); // convert as uint64_t

    EXPECT_EQ(TEST_CODE, common_code); // uint64_t == CommonCode
    EXPECT_EQ(common_code, TEST_CODE); // CommonCode == uint64_t
    EXPECT_EQ(common_code, common_code); // CommonCode == CommonCode

    EXPECT_NE(TEST_CODE + 1, common_code); // uint64_t != CommonCode
    EXPECT_NE(common_code, TEST_CODE + 1); // CommonCode != uint64_t
    EXPECT_NE(common_code, CommonCode::unsafe_create(TEST_CODE + 1)); // CommonCode != CommonCode

    EXPECT_LT(common_code, CommonCode::unsafe_create(TEST_CODE + 1)); // CommonCode < CommonCode
    EXPECT_GT(CommonCode::unsafe_create(TEST_CODE + 1), common_code); // CommonCode > CommonCode
}

TEST(CommonCode, code_verify) {
    BS::thread_pool pool;
    for (int head = 0; head < 16; ++head) {
        pool.push_task([](uint64_t head) {
            for (auto range : AllCases::instance().fetch()[head]) {
                auto code = head << 32 | range;
                EXPECT_TRUE(CommonCode::check(code)); // verify all valid code
            }
        }, head);
    }
    pool.wait_for_tasks();
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
    for (int head = 0; head < 16; ++head) {
        pool.push_task([&test_func](uint64_t head) {
            for (auto range : AllCases::instance().fetch()[head]) {
                test_func(CommonCode::unsafe_create(head << 32 | range));
            }
        }, head);
    }
    pool.wait_for_tasks();
}

TEST(CommonCode, code_convert) {
    auto common_code = CommonCode::create(TEST_CODE).value();
    EXPECT_EQ(common_code.unwrap(), TEST_CODE);
    EXPECT_EQ(common_code.to_string(), TEST_CODE_STR);
    EXPECT_EQ(common_code.to_raw_code(), RawCode(common_code));
    EXPECT_EQ(common_code.to_short_code(), ShortCode(common_code));

    auto code_shorten = common_code.to_string(true);
    EXPECT_EQ(CommonCode::from_string(code_shorten), common_code); // l-value
    EXPECT_EQ(CommonCode::from_string(std::move(code_shorten)), common_code); // r-value

    auto code_normal = common_code.to_string(false);
    EXPECT_EQ(CommonCode::from_string(code_normal), common_code); // l-value
    EXPECT_EQ(CommonCode::from_string(std::move(code_normal)), common_code); // r-value
}

TEST(CommonCode, initializate) {
    EXPECT_TRUE(CommonCode::create(TEST_CODE).has_value());
    EXPECT_EQ(CommonCode::create(TEST_CODE), TEST_CODE);
    EXPECT_EQ(CommonCode::unsafe_create(TEST_CODE), TEST_CODE);

    auto common_code = CommonCode::create(TEST_CODE).value();
    EXPECT_EQ(CommonCode(RawCode(common_code)), TEST_CODE);
    EXPECT_EQ(CommonCode(ShortCode(common_code)), TEST_CODE);

    EXPECT_TRUE(CommonCode::from_string(TEST_CODE_STR).has_value());
    EXPECT_EQ(CommonCode::from_string(TEST_CODE_STR), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode::from_string(std::string(TEST_CODE_STR)), TEST_CODE); // r-value

    auto raw_code = RawCode(common_code);
    auto raw_code_val = raw_code.unwrap();
    EXPECT_EQ(CommonCode::from_raw_code(raw_code), TEST_CODE);
    EXPECT_TRUE(CommonCode::from_raw_code(raw_code_val).has_value());
    EXPECT_EQ(CommonCode::from_raw_code(raw_code_val), TEST_CODE);

    auto short_code = ShortCode(common_code);
    auto short_code_val = short_code.unwrap();
    auto short_code_str = short_code.to_string();
    EXPECT_TRUE(CommonCode::from_short_code(short_code_val).has_value());
    EXPECT_TRUE(CommonCode::from_short_code(short_code_str).has_value());
    EXPECT_TRUE(CommonCode::from_short_code(std::string(short_code_str)).has_value());

    EXPECT_EQ(CommonCode::from_short_code(short_code_val), TEST_CODE);
    EXPECT_EQ(CommonCode::from_short_code(short_code), TEST_CODE);
    EXPECT_EQ(CommonCode::from_short_code(short_code_str), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode::from_short_code(std::string(short_code_str)), TEST_CODE); // r-value
}

TEST(CommonCode, DISABLED_global) {
    std::vector<uint64_t> common_codes;
    common_codes.reserve(ALL_CASES_NUM_);
    for (uint64_t head = 0; head < 16; ++head) {
        for (auto range : AllCases::instance().fetch()[head]) {
            common_codes.emplace_back(head << 32 | range);
        }
    }

    BS::thread_pool pool;
    auto futures = pool.parallelize_loop(0x10'0000'0000, [](uint64_t start, uint64_t end) {
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
    for (size_t i = 0; i < futures.size(); ++i) {
        auto data = futures[i].get();
        result.insert(result.end(), data.begin(), data.end()); // combine sections
    }
    pool.wait_for_tasks();
    EXPECT_EQ(result, common_codes);
}
