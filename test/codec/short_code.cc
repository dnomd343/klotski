#include <thread>
#include <algorithm>
#include "all_cases.h"
#include "short_code.h"
#include "gtest/gtest.h"

using klotski::AllCases;
using klotski::ShortCode;
using klotski::CommonCode;
using klotski::BasicRanges;

const static uint32_t TEST_CODE = 4091296;
const static std::string TEST_CODE_STR = "4WVE1";

inline void SHOULD_PANIC(const std::function<void(void)> &func) {
    bool panic_flag = false;
    try {
        func();
    } catch (klotski::ShortCodeException&) {
        panic_flag = true;
    }
    EXPECT_EQ(panic_flag, true);
}

TEST(ShortCode, invalid) {
    EXPECT_NE(ShortCode::check(-1), true); // out of short code range
    EXPECT_NE(ShortCode::check(29670987), true); // out of short code range
    SHOULD_PANIC([](){ ShortCode::from_string("R50EH"); }); // with invalid `0`
    SHOULD_PANIC([](){ ShortCode::from_string("123456"); }); // length != 5
    SHOULD_PANIC([](){ ShortCode::from_string("Z9EFV"); }); // out of short code range
}

TEST(ShortCode, speed_up) {
    std::thread threads[4];

    /// speed up to normal mode
    EXPECT_EQ(BasicRanges::status(), BasicRanges::NO_INIT);
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
    EXPECT_EQ(AllCases::status(), AllCases::NO_INIT);
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

TEST(ShortCode, code_verify) {
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            auto code = (uint32_t)ShortCode::from_common_code(head << 32 | range);
            EXPECT_EQ(ShortCode::check(code), true); // test static `check` interface
            auto tmp = ShortCode::unsafe_create(code); // test dynamic `valid` interface
            EXPECT_EQ(tmp.valid(), true);
        }
    };
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // ensure that short code fast mode enabled
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(ShortCode, code_string) {
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
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // ensure that short code fast mode enabled
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(ShortCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT -> " << ShortCode(TEST_CODE) << std::endl; // ostream test
    std::cout.clear();
    EXPECT_EQ(ShortCode(TEST_CODE), ShortCode(TEST_CODE)); // operator `==`
    EXPECT_EQ((uint32_t)ShortCode(TEST_CODE), TEST_CODE); // convert as uint64_t
}

TEST(ShortCode, code_convert) {
    EXPECT_STREQ(ShortCode(TEST_CODE).to_string().c_str(), TEST_CODE_STR.c_str());
    EXPECT_EQ(ShortCode(ShortCode(TEST_CODE).to_string()), ShortCode(TEST_CODE));
    EXPECT_EQ(ShortCode(TEST_CODE).to_common_code(), CommonCode::from_short_code(TEST_CODE));
    EXPECT_EQ(ShortCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(ShortCode, constructors) {
    EXPECT_EQ(ShortCode(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode(TEST_CODE_STR).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode(CommonCode::from_short_code(TEST_CODE)).unwrap(), TEST_CODE);
}

TEST(ShortCode, initializate) {
    EXPECT_EQ(ShortCode::create(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode::from_string(TEST_CODE_STR).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode::unsafe_create(TEST_CODE).unwrap(), TEST_CODE);

    auto common_code = CommonCode::from_short_code(TEST_CODE);
    EXPECT_EQ(ShortCode::from_common_code(common_code).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode::from_common_code(common_code.unwrap()).unwrap(), TEST_CODE);
    EXPECT_EQ(ShortCode::from_common_code(common_code.to_string()).unwrap(), TEST_CODE);
}
