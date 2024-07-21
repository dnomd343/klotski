#include <gtest/gtest.h>
#include <BS_thread_pool.hpp>

#include "raw_code/raw_code.h"
#include "all_cases/all_cases.h"
#include "common_code/common_code.h"

using klotski::codec::RawCode;
using klotski::cases::AllCases;

TEST(RawCode, vertical_mirror) {
    auto raw_code_a = RawCode::unsafe_create(0x0'FC0'480'6DB'FC0'480); // invalid case for test
    auto raw_code_b1 = RawCode::from_common_code(0x4FEA13400).value();
    auto raw_code_b2 = RawCode::from_common_code(0x8346AFC00).value();

    // EXPECT_TRUE(raw_code_a.is_vertical_mirror());
    // EXPECT_TRUE(raw_code_a.is_vertical_mirror(raw_code_a));
    EXPECT_EQ(raw_code_a.to_vertical_mirror(), raw_code_a);

    EXPECT_FALSE(raw_code_b1.is_vertical_mirror());
    EXPECT_FALSE(raw_code_b2.is_vertical_mirror());
    // EXPECT_TRUE(raw_code_b1.is_vertical_mirror(raw_code_b2));
    // EXPECT_TRUE(raw_code_b2.is_vertical_mirror(raw_code_b1));
    EXPECT_EQ(raw_code_b1.to_vertical_mirror(), raw_code_b2);
    EXPECT_EQ(raw_code_b2.to_vertical_mirror(), raw_code_b1);

    // EXPECT_FALSE(raw_code_a.is_vertical_mirror(raw_code_b1));
    // EXPECT_FALSE(raw_code_a.is_vertical_mirror(raw_code_b2));
    // EXPECT_FALSE(raw_code_b1.is_vertical_mirror(raw_code_a));
    // EXPECT_FALSE(raw_code_b2.is_vertical_mirror(raw_code_a));
}

TEST(RawCode, horizontal_mirror) {
    auto raw_code_a = RawCode::from_common_code(0x1A9BF0C00).value();
    auto raw_code_b1 = RawCode::from_common_code(0x4FEA13400).value();
    auto raw_code_b2 = RawCode::from_common_code(0x6BFA47000).value();

    EXPECT_TRUE(raw_code_a.is_horizontal_mirror());
    // EXPECT_TRUE(raw_code_a.is_horizontal_mirror(raw_code_a));
    EXPECT_EQ(raw_code_a.to_horizontal_mirror(), raw_code_a);

    EXPECT_FALSE(raw_code_b1.is_horizontal_mirror());
    EXPECT_FALSE(raw_code_b2.is_horizontal_mirror());
    // EXPECT_TRUE(raw_code_b1.is_horizontal_mirror(raw_code_b2));
    // EXPECT_TRUE(raw_code_b2.is_horizontal_mirror(raw_code_b1));
    EXPECT_EQ(raw_code_b1.to_horizontal_mirror(), raw_code_b2);
    EXPECT_EQ(raw_code_b2.to_horizontal_mirror(), raw_code_b1);

    // EXPECT_FALSE(raw_code_a.is_horizontal_mirror(raw_code_b1));
    // EXPECT_FALSE(raw_code_a.is_horizontal_mirror(raw_code_b2));
    // EXPECT_FALSE(raw_code_b1.is_horizontal_mirror(raw_code_a));
    // EXPECT_FALSE(raw_code_b2.is_horizontal_mirror(raw_code_a));
}

TEST(RawCode, code_vertical_mirror) {
    auto test_func = [](RawCode code) {
        auto mirror = code.to_vertical_mirror();
        EXPECT_TRUE(RawCode::check(mirror.unwrap()));
        EXPECT_EQ(mirror.to_common_code().to_raw_code(), mirror);
        EXPECT_EQ(mirror.to_vertical_mirror(), code);
        EXPECT_FALSE(mirror.is_vertical_mirror());
        EXPECT_NE(code, mirror);
    };

    BS::thread_pool pool;
    pool.detach_sequence(0, 16, [&test_func](const uint64_t head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            test_func(RawCode::from_common_code(head << 32 | range).value());
        }
    });
    pool.wait();
}

TEST(RawCode, code_horizontal_mirror) {
    auto test_func = [](RawCode code) {
        auto mirror = code.to_horizontal_mirror();
        EXPECT_TRUE(RawCode::check(mirror.unwrap()));
        EXPECT_EQ(mirror.to_common_code().to_raw_code(), mirror);
        EXPECT_EQ(mirror.to_horizontal_mirror(), code);
        if (mirror.is_horizontal_mirror()) {
            EXPECT_EQ(code, mirror);
        } else {
            EXPECT_NE(code, mirror);
        }
    };

    BS::thread_pool pool;
    pool.detach_sequence(0, 16, [&test_func](const uint64_t head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            test_func(RawCode::from_common_code(head << 32 | range).value());
        }
    });
    pool.wait();
}
