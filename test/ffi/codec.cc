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

const static char TEST_SHORT_CODE_STR_ERR[] = "R50EH";
const static char TEST_COMMON_CODE_STR_ERR[] = "123J432A9";

const static uint64_t TEST_MIRROR_V = 0x0'FC0'480'6DB'FC0'480;
const static uint64_t TEST_MIRROR_V1 = RawCode::from_common_code(0x4FEA13400).unwrap();
const static uint64_t TEST_MIRROR_V2 = RawCode::from_common_code(0x8346AFC00).unwrap();

const static uint64_t TEST_MIRROR_H = RawCode::from_common_code(0x1A9BF0C00).unwrap();
const static uint64_t TEST_MIRROR_H1 = RawCode::from_common_code(0x4FEA13400).unwrap();
const static uint64_t TEST_MIRROR_H2 = RawCode::from_common_code(0x6BFA47000).unwrap();

TEST(FFI, codec_warm_up) {
    /// short code normal mode check
    EXPECT_EQ(is_short_code_available(), BasicRanges::status() == BasicRanges::AVAILABLE);
    short_code_enable();
    EXPECT_EQ(is_short_code_available(), true);
    EXPECT_EQ(is_short_code_available(), BasicRanges::status() == BasicRanges::AVAILABLE);

    /// short code fast mode check
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

TEST(FFI, codec_string) {
    EXPECT_EQ(SHORT_CODE_STR_SIZE, 6);
    EXPECT_EQ(COMMON_CODE_STR_SIZE, 10);

    /// short code string verify
    uint32_t short_code;
    char short_code_buffer[SHORT_CODE_STR_SIZE];
    EXPECT_EQ(short_code_to_string(TEST_SHORT_CODE_ERR, short_code_buffer), false);
    EXPECT_EQ(short_code_from_string(TEST_SHORT_CODE_STR_ERR, &short_code), false);

    EXPECT_EQ(short_code_to_string(TEST_SHORT_CODE_OK, short_code_buffer), true);
    EXPECT_EQ(short_code_from_string(short_code_buffer, &short_code), true);
    EXPECT_EQ(short_code, TEST_SHORT_CODE_OK);

    /// common code string verify
    uint64_t common_code;
    char common_code_buffer[COMMON_CODE_STR_SIZE];
    EXPECT_EQ(common_code_to_string(TEST_COMMON_CODE_ERR, common_code_buffer), false);
    EXPECT_EQ(common_code_from_string(TEST_COMMON_CODE_STR_ERR, &common_code), false);

    EXPECT_EQ(common_code_to_string(TEST_COMMON_CODE_OK, common_code_buffer), true);
    EXPECT_EQ(common_code_from_string(common_code_buffer, &common_code), true);
    EXPECT_EQ(common_code, TEST_COMMON_CODE_OK);

    EXPECT_EQ(common_code_to_string_shorten(TEST_COMMON_CODE_OK, common_code_buffer), true);
    EXPECT_EQ(common_code_from_string(common_code_buffer, &common_code), true);
    EXPECT_EQ(common_code, TEST_COMMON_CODE_OK);
}

TEST(FFI, codec_convert) {
    uint64_t raw_code;
    uint32_t short_code;
    uint64_t common_code;

    /// raw code -> short code
    EXPECT_EQ(raw_code_to_short_code(TEST_RAW_CODE_ERR, &short_code), false);
    EXPECT_EQ(raw_code_to_short_code(TEST_RAW_CODE_OK, &short_code), true);
    EXPECT_EQ(TEST_SHORT_CODE_OK, short_code);

    /// short code -> raw code
    EXPECT_EQ(short_code_to_raw_code(TEST_SHORT_CODE_ERR, &raw_code), false);
    EXPECT_EQ(short_code_to_raw_code(TEST_SHORT_CODE_OK, &raw_code), true);
    EXPECT_EQ(TEST_RAW_CODE_OK, raw_code);

    /// raw code -> common code
    EXPECT_EQ(raw_code_to_common_code(TEST_RAW_CODE_ERR, &common_code), false);
    EXPECT_EQ(raw_code_to_common_code(TEST_RAW_CODE_OK, &common_code), true);
    EXPECT_EQ(TEST_COMMON_CODE_OK, common_code);

    /// common code -> raw code
    EXPECT_EQ(common_code_to_raw_code(TEST_COMMON_CODE_ERR, &raw_code), false);
    EXPECT_EQ(common_code_to_raw_code(TEST_COMMON_CODE_OK, &raw_code), true);
    EXPECT_EQ(TEST_RAW_CODE_OK, raw_code);

    /// short code -> common code
    EXPECT_EQ(short_code_to_common_code(TEST_SHORT_CODE_ERR, &common_code), false);
    EXPECT_EQ(short_code_to_common_code(TEST_SHORT_CODE_OK, &common_code), true);
    EXPECT_EQ(TEST_COMMON_CODE_OK, common_code);

    /// common code -> short code
    EXPECT_EQ(common_code_to_short_code(TEST_COMMON_CODE_ERR, &short_code), false);
    EXPECT_EQ(common_code_to_short_code(TEST_COMMON_CODE_OK, &short_code), true);
    EXPECT_EQ(TEST_SHORT_CODE_OK, short_code);
}

TEST(FFI, codec_convert_unsafe) {
    /// raw code <---> short code
    EXPECT_EQ(raw_code_to_short_code_unsafe(TEST_RAW_CODE_OK), TEST_SHORT_CODE_OK);
    EXPECT_EQ(short_code_to_raw_code_unsafe(TEST_SHORT_CODE_OK), TEST_RAW_CODE_OK);

    /// raw code <---> common code
    EXPECT_EQ(raw_code_to_common_code_unsafe(TEST_RAW_CODE_OK), TEST_COMMON_CODE_OK);
    EXPECT_EQ(common_code_to_raw_code_unsafe(TEST_COMMON_CODE_OK), TEST_RAW_CODE_OK);

    /// short code <---> common code
    EXPECT_EQ(short_code_to_common_code_unsafe(TEST_SHORT_CODE_OK), TEST_COMMON_CODE_OK);
    EXPECT_EQ(common_code_to_short_code_unsafe(TEST_COMMON_CODE_OK), TEST_SHORT_CODE_OK);
}

TEST(FFI, codec_mirror) {
    bool result;
    uint64_t raw_code;

    /// test of safe version
    EXPECT_EQ(is_vertical_mirror(TEST_RAW_CODE_ERR, &result), false);
    EXPECT_EQ(is_horizontal_mirror(TEST_RAW_CODE_ERR, &result), false);
    EXPECT_EQ(to_vertical_mirror(TEST_RAW_CODE_ERR, &raw_code), false);
    EXPECT_EQ(to_horizontal_mirror(TEST_RAW_CODE_ERR, &raw_code), false);

    EXPECT_EQ(is_vertical_mirror(TEST_RAW_CODE_OK, &result), true);
    EXPECT_EQ(result, false); // function always return false
    EXPECT_EQ(to_vertical_mirror(TEST_MIRROR_V1, &raw_code), true);
    EXPECT_EQ(raw_code, TEST_MIRROR_V2);
    EXPECT_EQ(to_vertical_mirror(TEST_MIRROR_V2, &raw_code), true);
    EXPECT_EQ(raw_code, TEST_MIRROR_V1);

    EXPECT_EQ(is_horizontal_mirror(TEST_MIRROR_H, &result), true);
    EXPECT_EQ(result, true);
    EXPECT_EQ(is_horizontal_mirror(TEST_MIRROR_H1, &result), true);
    EXPECT_EQ(result, false);
    EXPECT_EQ(is_horizontal_mirror(TEST_MIRROR_H2, &result), true);
    EXPECT_EQ(result, false);
    EXPECT_EQ(to_horizontal_mirror(TEST_MIRROR_H1, &raw_code), true);
    EXPECT_EQ(raw_code, TEST_MIRROR_H2);
    EXPECT_EQ(to_horizontal_mirror(TEST_MIRROR_H2, &raw_code), true);
    EXPECT_EQ(raw_code, TEST_MIRROR_H1);

    /// test of unsafe version
    EXPECT_EQ(is_vertical_mirror_unsafe(TEST_MIRROR_V), true);
    EXPECT_EQ(is_vertical_mirror_unsafe(TEST_MIRROR_V1), false);
    EXPECT_EQ(is_vertical_mirror_unsafe(TEST_MIRROR_V2), false);

    EXPECT_EQ(is_horizontal_mirror_unsafe(TEST_MIRROR_H), true);
    EXPECT_EQ(is_horizontal_mirror_unsafe(TEST_MIRROR_H1), false);
    EXPECT_EQ(is_horizontal_mirror_unsafe(TEST_MIRROR_H2), false);

    EXPECT_EQ(to_vertical_mirror_unsafe(TEST_MIRROR_V), TEST_MIRROR_V);
    EXPECT_EQ(to_vertical_mirror_unsafe(TEST_MIRROR_V1), TEST_MIRROR_V2);
    EXPECT_EQ(to_vertical_mirror_unsafe(TEST_MIRROR_V2), TEST_MIRROR_V1);

    EXPECT_EQ(to_horizontal_mirror_unsafe(TEST_MIRROR_H), TEST_MIRROR_H);
    EXPECT_EQ(to_horizontal_mirror_unsafe(TEST_MIRROR_H1), TEST_MIRROR_H2);
    EXPECT_EQ(to_horizontal_mirror_unsafe(TEST_MIRROR_H2), TEST_MIRROR_H1);
}
