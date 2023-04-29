#include <future>
#include <thread>
#include <algorithm>
#include <unordered_set>
#include "all_cases.h"
#include "tiny_pool.h"
#include "common_code.h"
#include "gtest/gtest.h"
#include "range_split.h"

#define SHOULD_PANIC(FUNC) \
    try { \
        FUNC; EXPECT_STREQ("should panic", "but no panic"); \
    } catch (klotski::CommonCodeExp&) {}

using klotski::RawCode;
using klotski::AllCases;
using klotski::ShortCode;
using klotski::CommonCode;
using klotski::ALL_CASES_SIZE;
using klotski::ALL_CASES_SIZE_SUM;

const static uint64_t TEST_CODE = 0x1'A9BF'0C00;
const static std::string TEST_CODE_STR = "1A9BF0C00";

TEST(CommonCode, hash) {
    auto tmp = std::unordered_set<CommonCode>{ CommonCode(TEST_CODE) };
    EXPECT_EQ(tmp.size(), 1);
}

TEST(CommonCode, validity) {
    EXPECT_NE(CommonCode::check(0x3'A9'BF'0C'00), true); // invalid 2x2 block
    EXPECT_NE(CommonCode::check(0x1'D9'BF'0C'00), true); // invalid block range
    EXPECT_NE(CommonCode::check(0x1'A9'BF'FC'00), true); // less than 2 space
    EXPECT_NE(CommonCode::check(0x1'A0'BF'0C'01), true); // low bits not fill zero

    SHOULD_PANIC(CommonCode::create(0x123456789)) // invalid code
    SHOULD_PANIC(CommonCode::from_string("0123456789")) // length > 9
    SHOULD_PANIC(CommonCode::from_string("123J432A9")) // with invalid `J`
}

TEST(CommonCode, code_verify) { // test all layout
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            uint64_t code = head << 32 | range;
            EXPECT_EQ(CommonCode::check(code), true); // test static `check` interface
            auto tmp = CommonCode::unsafe_create(code);
            EXPECT_EQ(tmp.valid(), true); // test dynamic `valid` interface
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
}

TEST(CommonCode, code_string) { // test all string code
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

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
}

TEST(CommonCode, operators) {
    std::cout.setstate(std::ios::failbit); // hide std::cout content
    std::cout << "TEST OUTPUT -> " << CommonCode(TEST_CODE) << std::endl; // ostream test
    std::cout.clear();
    EXPECT_EQ((uint64_t)CommonCode(TEST_CODE), TEST_CODE); // convert as uint64_t

    EXPECT_EQ(TEST_CODE, CommonCode(TEST_CODE)); // uint64_t == CommonCode
    EXPECT_EQ(CommonCode(TEST_CODE), TEST_CODE); // CommonCode == uint64_t
    EXPECT_EQ(CommonCode(TEST_CODE), CommonCode(TEST_CODE)); // CommonCode == CommonCode

    EXPECT_NE(TEST_CODE + 1, CommonCode(TEST_CODE)); // uint64_t != CommonCode
    EXPECT_NE(CommonCode(TEST_CODE), TEST_CODE + 1); // CommonCode != uint64_t
    EXPECT_NE(CommonCode(TEST_CODE), CommonCode::unsafe_create(TEST_CODE + 1)); // CommonCode != CommonCode

    EXPECT_LT(CommonCode(TEST_CODE), CommonCode::unsafe_create(TEST_CODE + 1)); // CommonCode < CommonCode
    EXPECT_GT(CommonCode::unsafe_create(TEST_CODE + 1), CommonCode(TEST_CODE)); // CommonCode > CommonCode
}

TEST(CommonCode, code_convert) {
    EXPECT_STREQ(CommonCode(TEST_CODE).to_string().c_str(), TEST_CODE_STR.c_str());
    EXPECT_EQ(CommonCode(CommonCode(TEST_CODE).to_string(true)), CommonCode(TEST_CODE));
    EXPECT_EQ(CommonCode(CommonCode(TEST_CODE).to_string(false)), CommonCode(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).to_raw_code(), RawCode::from_common_code(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).to_short_code(), ShortCode::from_common_code(TEST_CODE));
    EXPECT_EQ(CommonCode(TEST_CODE).unwrap(), TEST_CODE);
}

TEST(CommonCode, batch_convert) {
    auto test = [](uint64_t head) {
        std::vector<RawCode> raw_codes;
        std::vector<ShortCode> short_codes;
        std::vector<CommonCode> common_codes;
        raw_codes.reserve(ALL_CASES_SIZE[head]);
        short_codes.reserve(ALL_CASES_SIZE[head]);
        common_codes.reserve(ALL_CASES_SIZE[head]);

        for (auto &&range : AllCases::fetch()[head]) {
            common_codes.emplace_back(head << 32 | range);
            raw_codes.emplace_back(common_codes.back());
            short_codes.emplace_back(common_codes.back());
        }
        EXPECT_EQ(common_codes, CommonCode::convert(raw_codes));
        EXPECT_EQ(common_codes, CommonCode::convert(short_codes));
    };

    std::thread threads[16];
    ShortCode::speed_up(ShortCode::FAST);
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) { t.join(); }
}

TEST(CommonCode, constructors) {
    EXPECT_EQ(CommonCode(TEST_CODE).unwrap(), TEST_CODE);

    EXPECT_EQ(CommonCode(TEST_CODE_STR).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode(std::string(TEST_CODE_STR)).unwrap(), TEST_CODE); // r-value

    auto raw_code = RawCode::from_common_code(TEST_CODE);
    EXPECT_EQ(CommonCode(raw_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode(RawCode::from_common_code(TEST_CODE)).unwrap(), TEST_CODE); // r-value

    auto short_code = ShortCode::from_common_code(TEST_CODE);
    EXPECT_EQ(CommonCode(short_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode(ShortCode::from_common_code(TEST_CODE)).unwrap(), TEST_CODE); // r-value
}

TEST(CommonCode, initializate) {
    EXPECT_EQ(CommonCode::create(TEST_CODE).unwrap(), TEST_CODE);
    EXPECT_EQ(CommonCode::unsafe_create(TEST_CODE).unwrap(), TEST_CODE);

    EXPECT_EQ(CommonCode::from_string(TEST_CODE_STR).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode::from_string(std::string(TEST_CODE_STR)).unwrap(), TEST_CODE); // r-value

    auto raw_code = RawCode::from_common_code(TEST_CODE);

    EXPECT_EQ(CommonCode::from_raw_code(raw_code.unwrap()).unwrap(), TEST_CODE);

    EXPECT_EQ(CommonCode::from_raw_code(raw_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode::from_raw_code(
        RawCode::from_common_code(TEST_CODE) // r-value
    ).unwrap(), TEST_CODE);

    auto short_code = ShortCode::from_common_code(TEST_CODE);
    auto short_code_string = short_code.to_string();

    EXPECT_EQ(CommonCode::from_short_code(short_code.unwrap()).unwrap(), TEST_CODE);

    EXPECT_EQ(CommonCode::from_short_code(short_code).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode::from_short_code(
        ShortCode::from_common_code(TEST_CODE) // r-value
    ).unwrap(), TEST_CODE);

    EXPECT_EQ(CommonCode::from_short_code(short_code_string).unwrap(), TEST_CODE); // l-value
    EXPECT_EQ(CommonCode::from_short_code(short_code.to_string()).unwrap(), TEST_CODE); // r-value
}

TEST(CommonCode, DISABLED_global) {
    auto search = [](uint64_t start, uint64_t end) -> std::vector<uint64_t> {
        std::vector<uint64_t> archive;
        for (uint64_t common_code = start; common_code < end; ++common_code) {
            if (CommonCode::check(common_code)) {
                archive.emplace_back(common_code); // valid common code
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
