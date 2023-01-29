#include "all_cases.h"
#include "common_code.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::AllCases;

const static uint64_t TEST_CODE = 0x1A9BF0C00;
const static std::string TEST_CODE_STR = "1A9BF0C00";

TEST(CommonCode, code_verify) {
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            uint64_t code = head << 32 | range;
            EXPECT_EQ(CommonCode::check(code), true); // test static `check` interface
            auto tmp = CommonCode::unsafe_create(code); // test dynamic `valid` interface
            EXPECT_EQ(tmp.valid(), true);
        }
    }
}

TEST(CommonCode, code_string) {
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            uint64_t code = head << 32 | range;
            auto common_code = CommonCode::unsafe_create(code);
            EXPECT_EQ(CommonCode::from_string(common_code.to_string()), common_code);
        }
    }
}

TEST(CommonCode, constructors) {
    EXPECT_EQ(CommonCode(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode(TEST_CODE_STR).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode(RawCode::from_common_code(TEST_CODE)).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode(ShortCode::from_common_code(TEST_CODE)).unwrap(), TEST_CODE);
}

TEST(CommonCode, operators) {
    EXPECT_EQ(CommonCode(TEST_CODE), CommonCode(TEST_CODE)); // operator `==`
    std::cout << CommonCode(TEST_CODE) << std::endl; // ostream test
    EXPECT_EQ((uint64_t)CommonCode(TEST_CODE), TEST_CODE); // convert as uint64_t
    EXPECT_EQ(CommonCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(CommonCode, code_convert) {
    EXPECT_EQ(CommonCode(CommonCode(TEST_CODE).to_string()), CommonCode(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).to_raw_code(), RawCode::from_common_code(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).to_short_code(), ShortCode::from_common_code(TEST_CODE));
}

TEST(CommonCode, initializate) {
    EXPECT_EQ(CommonCode::create(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode::from_string(TEST_CODE_STR).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode::unsafe_create(TEST_CODE).unwrap(), TEST_CODE);

    auto raw_code = RawCode::from_common_code(TEST_CODE);
    EXPECT_EQ(CommonCode::from_raw_code(raw_code).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode::from_raw_code(raw_code.unwrap()).unwrap(), TEST_CODE);

    auto short_code = ShortCode::from_common_code(TEST_CODE);
    EXPECT_EQ(CommonCode::from_short_code(short_code).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode::from_short_code(short_code.unwrap()).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode::from_short_code(short_code.to_string()).unwrap(), TEST_CODE);
}
