#include <thread>
#include "raw_code.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;

const static uint64_t TEST_CODE = 0x0603'EDF5'CAFF'F5E2;
const static uint64_t TEST_ERR_CODE = 0x0A34'182B'3810'2D21;

// TODO: test some invalid cases

TEST(RawCode, code_verify) {
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            auto code = (uint64_t)RawCode::from_common_code(head << 32 | range);
            EXPECT_EQ(RawCode::check(code), true); // test static `check` interface
            auto tmp = RawCode::unsafe_create(code); // test dynamic `valid` interface
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

TEST(RawCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT" << std::endl << RawCode(TEST_CODE); // ostream test
    std::cout.clear();
    EXPECT_EQ(RawCode(TEST_CODE), RawCode(TEST_CODE)); // operator `==`
    EXPECT_EQ((uint64_t)RawCode(TEST_CODE), TEST_CODE); // convert as uint64_t
}

TEST(RawCode, code_convert) {
    EXPECT_EQ(RawCode(TEST_CODE).to_common_code(), CommonCode::from_raw_code(TEST_CODE));
    EXPECT_EQ(RawCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(RawCode, constructors) {
    EXPECT_EQ(RawCode(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(RawCode(CommonCode::from_raw_code(TEST_CODE)).unwrap(), TEST_CODE);
}

TEST(RawCode, initializate) {
    EXPECT_EQ(RawCode::create(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(RawCode::unsafe_create(TEST_CODE).unwrap(), TEST_CODE);

    auto common_code = CommonCode::from_raw_code(TEST_CODE);
    EXPECT_EQ(RawCode::from_common_code(common_code).unwrap(), TEST_CODE);
    EXPECT_EQ(RawCode::from_common_code(common_code.unwrap()).unwrap(), TEST_CODE);
    EXPECT_EQ(RawCode::from_common_code(common_code.to_string()).unwrap(), TEST_CODE);
}
