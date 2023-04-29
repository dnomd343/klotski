#include <thread>
#include <unordered_set>
#include "raw_code.h"
#include "all_cases.h"
#include "gtest/gtest.h"

#define SHOULD_PANIC(FUNC) \
    try { \
        FUNC; EXPECT_STREQ("should panic", "but no panic"); \
    } catch (klotski::RawCodeExp&) {}

using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;
using klotski::ALL_CASES_SIZE;

const static uint64_t TEST_CODE = 0x0603'EDF5'CAFF'F5E2;

TEST(RawCode, hash) {
    auto tmp = std::unordered_set<RawCode>{ RawCode(TEST_CODE) };
    EXPECT_EQ(tmp.size(), 1);
}

TEST(RawCode, validity) {
    EXPECT_EQ(RawCode::check(0x0A34'182B'3810'2D21), false); // invalid code
    EXPECT_EQ(RawCode::check(0x8603'EDF5'CAFF'F5E2), false); // high 4-bits not zero

    SHOULD_PANIC(RawCode::create(0x0000'0000'0000'0000)) // invalid code
}

TEST(RawCode, code_verify) { // test all layouts
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            auto code = (uint64_t)RawCode::from_common_code(head << 32 | range);
            EXPECT_EQ(RawCode::check(code), true); // test static `check` interface
            auto tmp = RawCode::unsafe_create(code);
            EXPECT_EQ(tmp.valid(), true); // test dynamic `valid` interface
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
}

TEST(RawCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT" << std::endl << RawCode(TEST_CODE); // ostream test
    std::cout.clear();
    EXPECT_EQ((uint64_t)RawCode(TEST_CODE), TEST_CODE); // convert as uint64_t

    EXPECT_EQ(TEST_CODE, RawCode(TEST_CODE)); // uint64_t == RawCode
    EXPECT_EQ(RawCode(TEST_CODE), TEST_CODE); // RawCode == uint64_t
    EXPECT_EQ(RawCode(TEST_CODE), RawCode(TEST_CODE)); // RawCode == RawCode

    EXPECT_NE(TEST_CODE + 1, RawCode(TEST_CODE)); // uint64_t != RawCode
    EXPECT_NE(RawCode(TEST_CODE), TEST_CODE + 1); // RawCode != uint64_t
    EXPECT_NE(RawCode(TEST_CODE), RawCode::unsafe_create(TEST_CODE + 1)); // RawCode != RawCode
}

TEST(RawCode, code_convert) {
    EXPECT_EQ(RawCode(TEST_CODE).to_common_code(), CommonCode::from_raw_code(TEST_CODE));
    EXPECT_EQ(RawCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(RawCode, batch_convert) {
    auto test = [](uint64_t head) {
        std::vector<RawCode> raw_codes;
        std::vector<CommonCode> common_codes;
        raw_codes.reserve(ALL_CASES_SIZE[head]);
        common_codes.reserve(ALL_CASES_SIZE[head]);

        for (auto &&range : AllCases::fetch()[head]) {
            common_codes.emplace_back(head << 32 | range);
            raw_codes.emplace_back(common_codes.back());
        }
        EXPECT_EQ(raw_codes, RawCode::convert(common_codes));
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
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

TEST(RawCode, vertical_mirror) {
    auto raw_code_1 = RawCode::unsafe_create(0x0'FC0'480'6DB'FC0'480); // invalid layout only for test
    auto raw_code_2 = RawCode::from_common_code(0x4FEA13400);
    auto raw_code_2_ = RawCode::from_common_code(0x8346AFC00);

    EXPECT_EQ(raw_code_1.is_vertical_mirror(), true);
    EXPECT_EQ(raw_code_1.to_vertical_mirror(), raw_code_1);
    EXPECT_EQ(raw_code_1.is_vertical_mirror(raw_code_1), true); // l-value
    EXPECT_EQ(raw_code_1.is_vertical_mirror(raw_code_2), false); // l-value
    EXPECT_EQ(raw_code_1.is_vertical_mirror(static_cast<RawCode&&>(raw_code_1)), true); // r-value
    EXPECT_EQ(raw_code_1.is_vertical_mirror(static_cast<RawCode&&>(raw_code_2)), false); // r-value

    EXPECT_EQ(raw_code_2.is_vertical_mirror(), false);
    EXPECT_EQ(raw_code_2.to_vertical_mirror(), raw_code_2_);
    EXPECT_EQ(raw_code_2.is_vertical_mirror(raw_code_1), false); // l-value
    EXPECT_EQ(raw_code_2.is_vertical_mirror(raw_code_2_), true); // l-value
    EXPECT_EQ(raw_code_2.is_vertical_mirror(static_cast<RawCode&&>(raw_code_1)), false); // r-value
    EXPECT_EQ(raw_code_2.is_vertical_mirror(static_cast<RawCode&&>(raw_code_2_)), true); // r-value
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

TEST(RawCode, vertical_mirror_global) {
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            /// generate code and mirror layout
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = common_code.to_raw_code();
            auto raw_code_mirror = raw_code.to_vertical_mirror();

            /// verify vertical mirror
            EXPECT_EQ(raw_code_mirror.valid(), true);
            EXPECT_EQ(raw_code_mirror.to_common_code().to_raw_code(), raw_code_mirror);
            EXPECT_EQ(raw_code.is_vertical_mirror(), false);
            EXPECT_NE(raw_code, raw_code_mirror);
            EXPECT_EQ(raw_code_mirror.to_vertical_mirror(), raw_code);
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
}

TEST(RawCode, horizontal_mirror_global) {
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

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
}
