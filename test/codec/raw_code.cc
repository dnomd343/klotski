#include <thread>
#include "raw_code.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;

const static uint64_t TEST_CODE = 0x0603'EDF5'CAFF'F5E2;

TEST(RawCode, validity) {
    EXPECT_EQ(RawCode::check(0x0A34'182B'3810'2D21), false); // invalid code
    EXPECT_EQ(RawCode::check(0x8603'EDF5'CAFF'F5E2), false); // high 4-bits not zero
}

TEST(RawCode, code_verify) { // test all layouts
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            auto code = (uint64_t)RawCode::from_common_code(head << 32 | range);
            EXPECT_EQ(RawCode::check(code), true); // test static `check` interface
            auto tmp = RawCode::unsafe_create(code); // test dynamic `valid` interface
            EXPECT_EQ(tmp.valid(), true);
        }
    };
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(RawCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT" << std::endl << RawCode(TEST_CODE); // ostream test
    std::cout.clear();
    EXPECT_EQ(RawCode(TEST_CODE), TEST_CODE); // operator `==`
    EXPECT_EQ(RawCode(TEST_CODE), RawCode(TEST_CODE)); // operator `==`
    EXPECT_EQ((uint64_t)RawCode(TEST_CODE), TEST_CODE); // convert as uint64_t
}

TEST(RawCode, code_convert) {
    EXPECT_EQ(RawCode(TEST_CODE).to_common_code(), CommonCode::from_raw_code(TEST_CODE));
    EXPECT_EQ(RawCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(RawCode, constructors) {
    EXPECT_EQ(RawCode(TEST_CODE).unwrap(), TEST_CODE);
    auto common_code = CommonCode::from_raw_code(TEST_CODE);
    EXPECT_EQ(RawCode(common_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(RawCode(CommonCode::from_raw_code(TEST_CODE)).unwrap(), TEST_CODE); // r-value
}

TEST(RawCode, initializate) {
    EXPECT_EQ(RawCode::create(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(RawCode::unsafe_create(TEST_CODE).unwrap(), TEST_CODE);

    auto common_code = CommonCode::from_raw_code(TEST_CODE);
    auto common_code_string = common_code.to_string();

    EXPECT_EQ(RawCode::from_common_code(common_code.unwrap()).unwrap(), TEST_CODE);

    EXPECT_EQ(RawCode::from_common_code(common_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(RawCode::from_common_code(
        CommonCode::from_raw_code(TEST_CODE) // r-value
    ).unwrap(), TEST_CODE);

    EXPECT_EQ(RawCode::from_common_code(common_code_string).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(RawCode::from_common_code(common_code.to_string()).unwrap(), TEST_CODE); // r-value
}

TEST(RawCode, horizontal_mirror) {
    auto raw_code_1 = RawCode::from_common_code(0x1A9BF0C00);
    auto raw_code_2 = RawCode::from_common_code(0x4FEA13400);
    auto raw_code_2_ = RawCode::from_common_code(0x6BFA47000);

    EXPECT_EQ(raw_code_1.is_horizontal_mirror(), true);
    EXPECT_EQ(raw_code_1.to_horizontal_mirror(), raw_code_1);
    EXPECT_EQ(raw_code_1.is_horizontal_mirror(raw_code_1), true); // l-value
    EXPECT_EQ(raw_code_1.is_horizontal_mirror(raw_code_2), false); // l-value
    EXPECT_EQ(raw_code_1.is_horizontal_mirror(static_cast<RawCode&&>(raw_code_1)), true); // r-value
    EXPECT_EQ(raw_code_1.is_horizontal_mirror(static_cast<RawCode&&>(raw_code_2)), false); // r-value

    EXPECT_EQ(raw_code_2.is_horizontal_mirror(), false);
    EXPECT_EQ(raw_code_2.to_horizontal_mirror(), raw_code_2_);
    EXPECT_EQ(raw_code_2.is_horizontal_mirror(raw_code_1), false); // l-value
    EXPECT_EQ(raw_code_2.is_horizontal_mirror(raw_code_2_), true); // l-value
    EXPECT_EQ(raw_code_2.is_horizontal_mirror(static_cast<RawCode&&>(raw_code_1)), false); // r-value
    EXPECT_EQ(raw_code_2.is_horizontal_mirror(static_cast<RawCode&&>(raw_code_2_)), true); // r-value
}

TEST(RawCode, horizontal_mirror_global) {
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            /// generate code and mirror layout
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = common_code.to_raw_code();
            auto raw_code_mirror = raw_code.to_horizontal_mirror();

            /// verify horizontal mirror
            EXPECT_EQ(raw_code_mirror.valid(), true);
            EXPECT_EQ(raw_code_mirror.to_common_code().to_raw_code(), raw_code_mirror);
            if (raw_code.is_horizontal_mirror()) {
                EXPECT_EQ(raw_code, raw_code_mirror);
            } else {
                EXPECT_NE(raw_code, raw_code_mirror);
            }
            EXPECT_EQ(raw_code_mirror.to_horizontal_mirror(), raw_code);
        }
    };
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}
