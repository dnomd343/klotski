#include "klotski.h"
#include "all_cases.h"
#include "short_code.h"
#include "gtest/gtest.h"

using klotski::AllCases;
using klotski::ShortCode;
using klotski::BasicRanges;

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
