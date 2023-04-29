#include <future>
#include <thread>
#include <unordered_set>
#include "common.h"
#include "raw_code.h"
#include "all_cases.h"
#include "tiny_pool.h"
#include "gtest/gtest.h"
#include "range_split.h"

#define SHOULD_PANIC(FUNC) \
    try { \
        FUNC; EXPECT_STREQ("should panic", "but no panic"); \
    } catch (klotski::RawCodeExp&) {}

using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;
using klotski::ALL_CASES_SIZE;
using klotski::ALL_CASES_SIZE_SUM;
using klotski::Common::range_reverse;

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

/// NOTE: for RawCode global test
uint64_t raw_code_convert(uint64_t common_code) { // try to convert as raw code
    auto code = C_2x2 << (common_code >> 32) * 3;
    auto range = range_reverse((uint32_t)common_code);
    for (int addr = 0; range; range >>= 2) {
        while ((code >> addr) & 0b111 && addr < 60) {
            addr += 3;
        }
        if (addr >= 60) {
            return 0;
        }
        switch (range & 0b11) { // match low 2-bits
            case 0b01: // 1x2 block
                code |= C_1x2 << addr;
                break;
            case 0b10: // 2x1 block
                code |= C_2x1 << addr;
                break;
            case 0b11: // 1x1 block
                code |= C_1x1 << addr;
                break;
            case 0b00: // space
                addr += 3;
        }
    }
    return code;
}

TEST(RawCode, DISABLED_global) {
    auto search = [](uint64_t start, uint64_t end) -> std::vector<uint64_t> {
        std::vector<uint64_t> archive;
        for (uint64_t common_code = start; common_code < end; ++common_code) {
            if (RawCode::check(raw_code_convert(common_code))) {
                archive.emplace_back(common_code); // valid layout
            }
        }
        return archive;
    };

    auto pool = TinyPool();
    std::vector<std::future<std::vector<uint64_t>>> tasks;
    for (const auto &range : range_split(0, 0x10'0000'0000, 0x10'0000)) {
        tasks.emplace_back(
            pool.submit(search, range.first, range.second)
        );
    }
    pool.boot(); // running tasks

    std::vector<uint64_t> result;
    for (auto &tmp : tasks) {
        auto ret = tmp.get(); // release data
        result.insert(result.end(), ret.begin(), ret.end());
    }
    pool.join();

    auto all_cases = AllCases::release();
    for (uint32_t i = 0; i < ALL_CASES_SIZE_SUM; ++i) {
        EXPECT_EQ(all_cases[i], result[i]);
    }
}
