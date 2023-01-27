#include <cstdint>
#include "common.h"
#include "gtest/gtest.h"

using klotski::Common;

TEST(Utils, range_reverse) {
    EXPECT_EQ(Common::range_reverse((uint32_t)0x00000003), (uint32_t)0xC0000000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x0000000C), (uint32_t)0x30000000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x00000030), (uint32_t)0x0C000000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x000000C0), (uint32_t)0x03000000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x00000300), (uint32_t)0x00C00000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x00000C00), (uint32_t)0x00300000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x00003000), (uint32_t)0x000C0000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x0000C000), (uint32_t)0x00030000);

    EXPECT_EQ(Common::range_reverse((uint32_t)0x0000000F), (uint32_t)0xF0000000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x000000FF), (uint32_t)0xFF000000);
    EXPECT_EQ(Common::range_reverse((uint32_t)0x0000FFFF), (uint32_t)0xFFFF0000);

    EXPECT_EQ(Common::range_reverse((uint32_t)0x55555555), (uint32_t)0x55555555);
    EXPECT_EQ(Common::range_reverse((uint32_t)0xAAAAAAAA), (uint32_t)0xAAAAAAAA);
}

TEST(Utils, check_range) {
    EXPECT_NE(Common::check_range(1, 0xA9BF0C00), 0);
    EXPECT_EQ(Common::check_range(1, 0x0030FE6A), 0);

    EXPECT_NE(Common::check_range(4, 0xFEA13400), 0);
    EXPECT_EQ(Common::check_range(4, 0x001C4ABF), 0);

    EXPECT_EQ(Common::check_range(5, 0x004845D3), 5);
    EXPECT_EQ(Common::check_range(10, 0x003B7521), 8);
    EXPECT_EQ(Common::check_range(13, 0x000EC9D3), 9);
    EXPECT_EQ(Common::check_range(15, 0x00A3B724), 6);
}
