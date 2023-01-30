#include "klotski.h"
#include "raw_code.h"
#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::ShortCode;
using klotski::BasicRanges;

const static uint32_t TEST_SHORT_CODE_OK = 4091296;
const static uint64_t TEST_COMMON_CODE_OK = 0x1'A9BF'0C00;
const static uint64_t TEST_RAW_CODE_OK = 0x0603'EDF5'CAFF'F5E2;

const static uint32_t TEST_SHORT_CODE_ERR = 29670987;
const static uint64_t TEST_COMMON_CODE_ERR = 0x1'2190'2300;
const static uint64_t TEST_RAW_CODE_ERR = 0x0A34'182B'3810'2D21;

TEST(FFI, codec_warm_up) {
    // short code normal mode check
    EXPECT_EQ(is_short_code_available(), BasicRanges::status() == BasicRanges::AVAILABLE);
    short_code_enable();
    EXPECT_EQ(is_short_code_available(), true);
    EXPECT_EQ(is_short_code_available(), BasicRanges::status() == BasicRanges::AVAILABLE);

    // short code fast mode check
    EXPECT_EQ(is_short_code_available_fast(), AllCases::status() == AllCases::AVAILABLE);
    short_code_enable_fast();
    EXPECT_EQ(is_short_code_available_fast(), true);
    EXPECT_EQ(is_short_code_available_fast(), AllCases::status() == AllCases::AVAILABLE);
}

TEST(FFI, codec_checker) {
    EXPECT_EQ(raw_code_check(TEST_RAW_CODE_OK), true);
    EXPECT_NE(raw_code_check(TEST_RAW_CODE_ERR), true);

    EXPECT_EQ(short_code_check(TEST_SHORT_CODE_OK), true);
    EXPECT_NE(short_code_check(TEST_SHORT_CODE_ERR), true);

    EXPECT_EQ(common_code_check(TEST_COMMON_CODE_OK), true);
    EXPECT_NE(common_code_check(TEST_COMMON_CODE_ERR), true);
}
