#include <thread>
#include <algorithm>
#include "all_cases.h"
#include "common_code.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::ShortCode;
using klotski::CommonCode;

const static uint64_t TEST_CODE = 0x1'A9BF'0C00;
const static std::string TEST_CODE_STR = "1A9BF0C00";

inline void SHOULD_PANIC(const std::function<void(void)> &func) {
    bool panic_flag = false;
    try {
        func();
    } catch (klotski::CommonCodeException&) {
        panic_flag = true;
    }
    EXPECT_EQ(panic_flag, true);
}

TEST(CommonCode, invalid) {
    EXPECT_NE(CommonCode::check(0x3'A9'BF'0C'00), true); // invalid 2x2 block
    EXPECT_NE(CommonCode::check(0x1'D9'BF'0C'00), true); // invalid block range
    EXPECT_NE(CommonCode::check(0x1'A9'BF'FC'00), true); // less than 2 space
    EXPECT_NE(CommonCode::check(0x1'A0'BF'0C'01), true); // low bits not fill zero

    SHOULD_PANIC([](){ CommonCode::from_string("0123456789"); }); // length > 9
    SHOULD_PANIC([](){ CommonCode::from_string("123J432A9"); }); // with invalid `J`
}

TEST(CommonCode, code_verify) {
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            uint64_t code = head << 32 | range;
            EXPECT_EQ(CommonCode::check(code), true); // test static `check` interface
            auto tmp = CommonCode::unsafe_create(code); // test dynamic `valid` interface
            EXPECT_EQ(tmp.valid(), true);
        }
    };
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(CommonCode, code_string) {
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            std::string code_str;
            uint64_t code = head << 32 | range;
            auto common_code = CommonCode::unsafe_create(code);

            code_str = common_code.to_string(true); // with shorten
            EXPECT_LE(code_str.size(), 9); // length -> (0, 9]
            EXPECT_NE(code_str.size(), 0);
            if (code != 0) { // skip special code string `0`
                EXPECT_NE(code_str.back(), '0');
            }
            EXPECT_EQ(CommonCode::from_string(code_str), common_code);

            code_str = common_code.to_string(false); // without shorten
            EXPECT_EQ(code_str.size(), 9); // length = 9
            for (const auto &c : code_str) {
                EXPECT_EQ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'), true);
            }
            EXPECT_EQ(CommonCode::from_string(code_str), common_code); // test upper cases
            std::transform(code_str.begin(), code_str.end(), code_str.begin(), ::tolower);
            EXPECT_EQ(CommonCode::from_string(code_str), common_code); // test lower cases
        }
    };
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(CommonCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT -> " << CommonCode(TEST_CODE) << std::endl; // ostream test
    std::cout.clear();
    EXPECT_EQ(CommonCode(TEST_CODE), CommonCode(TEST_CODE)); // operator `==`
    EXPECT_EQ((uint64_t)CommonCode(TEST_CODE), TEST_CODE); // convert as uint64_t
}

TEST(CommonCode, code_convert) {
    EXPECT_STREQ(CommonCode(TEST_CODE).to_string().c_str(), TEST_CODE_STR.c_str());
    EXPECT_EQ(CommonCode(CommonCode(TEST_CODE).to_string()), CommonCode(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).to_raw_code(), RawCode::from_common_code(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).to_short_code(), ShortCode::from_common_code(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(CommonCode, constructors) {
    EXPECT_EQ(CommonCode(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode(TEST_CODE_STR).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode(RawCode::from_common_code(TEST_CODE)).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode(ShortCode::from_common_code(TEST_CODE)).unwrap(), TEST_CODE);
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
