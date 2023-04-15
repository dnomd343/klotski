#include <thread>
#include <algorithm>
#include <unordered_set>
#include "all_cases.h"
#include "short_code.h"
#include "gtest/gtest.h"

#define SHOULD_PANIC(FUNC) \
    try { \
        FUNC; EXPECT_STREQ("should panic", "but no panic"); \
    } catch (...) {}

using klotski::AllCases;
using klotski::ShortCode;
using klotski::CommonCode;
using klotski::BasicRanges;

using klotski::SHORT_CODE_LIMIT;
using klotski::ALL_CASES_SIZE_SUM;

const static uint32_t TEST_CODE = 4091296;
const static std::string TEST_CODE_STR = "4WVE1";

TEST(ShortCode, limit) {
    EXPECT_EQ(ALL_CASES_SIZE_SUM, SHORT_CODE_LIMIT);
}

TEST(ShortCode, hash) {
    auto tmp = std::unordered_set<ShortCode>{ ShortCode(TEST_CODE) };
    EXPECT_EQ(tmp.size(), 1);
}

TEST(ShortCode, validity) {
    EXPECT_NE(ShortCode::check(-1), true); // out of short code range
    EXPECT_NE(ShortCode::check(29670987), true); // out of short code range
    SHOULD_PANIC(ShortCode::from_string("R50EH")) // with invalid `0`
    SHOULD_PANIC(ShortCode::from_string("123456")) // length != 5
    SHOULD_PANIC(ShortCode::from_string("Z9EFV")) // out of short code range
}

TEST(ShortCode, speed_up) {
    std::thread threads[4];

    /// speed up to normal mode
    EXPECT_EQ(BasicRanges::status(), BasicRanges::NOT_INIT);
    for (auto &t : threads) {
        t = std::thread(ShortCode::speed_up, ShortCode::NORMAL);
    }
    usleep(1000); // wait 1ms -> avoid mutex unlocked
    EXPECT_EQ(BasicRanges::status(), BasicRanges::BUILDING);
    for (auto &t : threads) {
        t.join();
    }
    EXPECT_EQ(BasicRanges::status(), BasicRanges::AVAILABLE);

    /// speed up to fast mode
    EXPECT_EQ(AllCases::status(), AllCases::NOT_INIT);
    for (auto &t : threads) {
        t = std::thread(ShortCode::speed_up, ShortCode::FAST);
    }
    usleep(1000); // wait 1ms -> avoid mutex unlocked
    EXPECT_EQ(AllCases::status(), AllCases::BUILDING);
    for (auto &t : threads) {
        t.join();
    }
    EXPECT_EQ(AllCases::status(), AllCases::AVAILABLE);
}

TEST(ShortCode, code_verify) { // test all layout
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            auto code = (uint32_t)ShortCode::from_common_code(head << 32 | range);
            EXPECT_EQ(ShortCode::check(code), true); // test static `check` interface
            auto tmp = ShortCode::unsafe_create(code); // test dynamic `valid` interface
            EXPECT_EQ(tmp.valid(), true);
        }
    };
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        /// NOTE: ensure that short code fast mode enabled
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(ShortCode, code_string) { // test all string code
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            std::string code_str;
            auto short_code = ShortCode::from_common_code(head << 32 | range);

            code_str = short_code.to_string();
            EXPECT_EQ(code_str.size(), 5); // length = 5
            for (const auto &c : code_str) {
                EXPECT_EQ(
                    ((c >= '1' && c <= '9') || (c >= 'A' && c <= 'Z'))
                        && (c != 'I' && c != 'L' && c != 'O'), true
                );
            }
            EXPECT_EQ(ShortCode::from_string(code_str), short_code); // test upper cases
            std::transform(code_str.begin(), code_str.end(), code_str.begin(), ::tolower);
            EXPECT_EQ(ShortCode::from_string(code_str), short_code); // test lower cases
        }
    };
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(ShortCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT -> " << ShortCode(TEST_CODE) << std::endl; // ostream test
    std::cout.clear();
    EXPECT_EQ((uint32_t)ShortCode(TEST_CODE), TEST_CODE); // convert as uint64_t

    EXPECT_EQ(TEST_CODE, ShortCode(TEST_CODE)); // uint32_t == ShortCode
    EXPECT_EQ(ShortCode(TEST_CODE), TEST_CODE); // ShortCode == uint32_t
    EXPECT_EQ(ShortCode(TEST_CODE), ShortCode(TEST_CODE)); // ShortCode == ShortCode

    EXPECT_NE(TEST_CODE + 1, ShortCode(TEST_CODE)); // uint32_t != ShortCode
    EXPECT_NE(ShortCode(TEST_CODE), TEST_CODE + 1); // ShortCode != uint32_t
    EXPECT_NE(ShortCode(TEST_CODE), ShortCode::unsafe_create(TEST_CODE + 1)); // ShortCode != ShortCode
}

TEST(ShortCode, code_convert) {
    EXPECT_STREQ(ShortCode(TEST_CODE).to_string().c_str(), TEST_CODE_STR.c_str());
    EXPECT_EQ(ShortCode(ShortCode(TEST_CODE).to_string()), ShortCode(TEST_CODE));
    EXPECT_EQ(ShortCode(TEST_CODE).to_common_code(), CommonCode::from_short_code(TEST_CODE));
    EXPECT_EQ(ShortCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(ShortCode, constructors) {
    EXPECT_EQ(ShortCode(TEST_CODE).unwrap(), TEST_CODE);

    EXPECT_EQ(ShortCode(TEST_CODE_STR).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(ShortCode(std::string(TEST_CODE_STR)).unwrap(), TEST_CODE); // r-value

    auto common_code = CommonCode::from_short_code(TEST_CODE);
    EXPECT_EQ(ShortCode(common_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(ShortCode(CommonCode::from_short_code(TEST_CODE)).unwrap(), TEST_CODE); // r-value
}

TEST(ShortCode, initializate) {
    EXPECT_EQ(ShortCode::create(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode::unsafe_create(TEST_CODE).unwrap(), TEST_CODE);

    EXPECT_EQ(ShortCode::from_string(TEST_CODE_STR).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(ShortCode::from_string(std::string(TEST_CODE_STR)).unwrap(), TEST_CODE); // r-value

    auto common_code = CommonCode::from_short_code(TEST_CODE);
    auto common_code_string = common_code.to_string(false);

    EXPECT_EQ(ShortCode::from_common_code(common_code.unwrap()).unwrap(), TEST_CODE);

    EXPECT_EQ(ShortCode::from_common_code(common_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(ShortCode::from_common_code(
        CommonCode::from_short_code(TEST_CODE) // r-value
    ).unwrap(), TEST_CODE);

    EXPECT_EQ(ShortCode::from_common_code(common_code_string).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(ShortCode::from_common_code(common_code.to_string()).unwrap(), TEST_CODE); // r-value
}
