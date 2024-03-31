#include <algorithm>

#include "sample.h"
#include "exposer.h"
#include "all_cases.h"
#include "short_code.h"
#include "common_code.h"
#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::codec::SHORT_CODE_LIMIT;

static const auto TEST_THREAD_NUM = 256;

/// Forcibly modify private variables to reset state.
FORCIBLY_ACCESS(AllCases, available_, bool)
FORCIBLY_ACCESS(BasicRanges, available_, bool)

/// Reset basic ranges build state, note it is thread-unsafe.
void basic_ranges_reset() {
    exposer::BasicRanges_available_(BasicRanges::instance()) = false;
}

/// Reset all cases build state, note it is thread-unsafe.
void all_cases_reset() {
    exposer::AllCases_available_(AllCases::instance()) = false;
}

TEST(ShortCode, limit) {
    auto all_cases_num = std::accumulate(ALL_CASES_NUM.begin(), ALL_CASES_NUM.end(), 0);
    EXPECT_EQ(all_cases_num, SHORT_CODE_LIMIT);
}

TEST(ShortCode, validity) {
    EXPECT_FALSE(ShortCode::check(-1)); // out of short code range
    EXPECT_FALSE(ShortCode::check(29670987)); // out of short code range

    EXPECT_FALSE(ShortCode::create(SHORT_CODE_LIMIT).has_value()); // invalid code
    EXPECT_FALSE(ShortCode::from_string("R50EH").has_value()); // with invalid `0`
    EXPECT_FALSE(ShortCode::from_string("123456").has_value()); // length != 5
    EXPECT_FALSE(ShortCode::from_string("Z9EFV").has_value()); // out of short code range
}

TEST(ShortCode, operators) {
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);

    std::ostringstream tmp;
    tmp << short_code; // ostream capture
    EXPECT_EQ(tmp.str(), TEST_S_CODE_STR);
    EXPECT_EQ((uint32_t)short_code, TEST_S_CODE); // convert as uint32_t

    EXPECT_EQ(TEST_S_CODE, short_code); // uint32_t == ShortCode
    EXPECT_EQ(short_code, TEST_S_CODE); // ShortCode == uint32_t
    EXPECT_EQ(short_code, short_code); // ShortCode == ShortCode

    EXPECT_NE(TEST_S_CODE + 1, short_code); // uint32_t != ShortCode
    EXPECT_NE(short_code, TEST_S_CODE + 1); // ShortCode != uint32_t
    EXPECT_NE(short_code, ShortCode::unsafe_create(TEST_S_CODE + 1)); // ShortCode != ShortCode

    EXPECT_LT(short_code, ShortCode::unsafe_create(TEST_S_CODE + 1)); // ShortCode < ShortCode
    EXPECT_GT(ShortCode::unsafe_create(TEST_S_CODE + 1), short_code); // ShortCode > ShortCode
}

TEST(ShortCode, exporter) {
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);
    EXPECT_EQ(short_code.unwrap(), TEST_S_CODE);
    EXPECT_EQ(short_code.to_string(), TEST_S_CODE_STR);
    EXPECT_EQ(short_code.to_common_code(), TEST_C_CODE);
}

TEST(ShortCode, initializate) {
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);
    auto common_code = CommonCode::unsafe_create(TEST_C_CODE);

    // ShortCode(...)
    EXPECT_EQ(ShortCode(common_code), TEST_S_CODE);
    EXPECT_EQ(ShortCode(short_code), TEST_S_CODE); // l-value
    EXPECT_EQ(ShortCode(ShortCode(short_code)), TEST_S_CODE); // r-value

    // ShortCode::create(uint32_t)
    EXPECT_TRUE(ShortCode::create(TEST_S_CODE).has_value());
    EXPECT_FALSE(ShortCode::create(TEST_S_CODE_ERR).has_value());
    EXPECT_EQ(ShortCode::create(TEST_S_CODE), TEST_S_CODE);

    // ShortCode::unsafe_create(uint32_t)
    EXPECT_EQ(ShortCode::unsafe_create(TEST_S_CODE), TEST_S_CODE);

    // ShortCode::from_string(const std::string &)
    EXPECT_TRUE(ShortCode::from_string(TEST_S_CODE_STR).has_value());
    EXPECT_FALSE(ShortCode::from_string(TEST_S_CODE_STR_ERR).has_value());
    EXPECT_EQ(ShortCode::from_string(TEST_S_CODE_STR), TEST_S_CODE);

    // ShortCode::from_string(std::string &&)
    EXPECT_TRUE(ShortCode::from_string(TEST_S_CODE_STR_RV).has_value());
    EXPECT_FALSE(ShortCode::from_string(TEST_S_CODE_STR_ERR_RV).has_value());
    EXPECT_EQ(ShortCode::from_string(TEST_S_CODE_STR_RV), TEST_S_CODE);

    // ShortCode::from_common_code(CommonCode)
    EXPECT_EQ(ShortCode::from_common_code(common_code), TEST_S_CODE);

    // ShortCode::from_common_code(uint64_t)
    EXPECT_TRUE(ShortCode::from_common_code(TEST_C_CODE).has_value());
    EXPECT_FALSE(ShortCode::from_common_code(TEST_C_CODE_ERR).has_value());
    EXPECT_EQ(ShortCode::from_common_code(TEST_C_CODE), TEST_S_CODE);

    // ShortCode::from_common_code(const std::string &)
    EXPECT_TRUE(ShortCode::from_common_code(TEST_C_CODE_STR).has_value());
    EXPECT_FALSE(ShortCode::from_common_code(TEST_C_CODE_STR_ERR).has_value());
    EXPECT_EQ(ShortCode::from_common_code(TEST_C_CODE_STR), TEST_S_CODE);

    // ShortCode::from_common_code(std::string &&)
    EXPECT_TRUE(ShortCode::from_common_code(TEST_C_CODE_STR_RV).has_value());
    EXPECT_FALSE(ShortCode::from_common_code(TEST_C_CODE_STR_ERR_RV).has_value());
    EXPECT_EQ(ShortCode::from_common_code(TEST_C_CODE_STR_RV), TEST_S_CODE);
}

TEST(ShortCode, speed_up) {
    all_cases_reset();
    basic_ranges_reset();
    BS::thread_pool pool(TEST_THREAD_NUM);

    for (auto i = 0; i < TEST_THREAD_NUM; ++i) {
        pool.detach_task([]() {
            ShortCode::speed_up(false);
        });
    }
    EXPECT_FALSE(BasicRanges::instance().is_available());
    EXPECT_FALSE(AllCases::instance().is_available());
    pool.wait();
    EXPECT_TRUE(BasicRanges::instance().is_available());
    EXPECT_FALSE(AllCases::instance().is_available());

    for (auto i = 0; i < TEST_THREAD_NUM; ++i) {
        pool.detach_task([]() {
            ShortCode::speed_up(true);
        });
    }
    EXPECT_TRUE(BasicRanges::instance().is_available());
    EXPECT_FALSE(AllCases::instance().is_available());
    pool.wait();
    EXPECT_TRUE(BasicRanges::instance().is_available());
    EXPECT_TRUE(AllCases::instance().is_available());
}

TEST(ShortCode, code_verify) {
    BS::thread_pool pool;
    ShortCode::speed_up(true);
    pool.detach_sequence(0, 16, [](const uint64_t head) {
        std::vector<uint32_t> archive;
        for (auto range : AllCases::instance().fetch()[head]) {
            auto code = ShortCode::from_common_code(head << 32 | range);
            EXPECT_TRUE(code.has_value());
            EXPECT_TRUE(ShortCode::check(code->unwrap()));
            EXPECT_EQ(code->to_common_code(), head << 32 | range);
            archive.emplace_back(code->unwrap());
        }
        if (!archive.empty()) {
            EXPECT_TRUE(std::is_sorted(archive.begin(), archive.end())); // increasingly one by one
            EXPECT_EQ(archive[archive.size() - 1] - archive[0], archive.size() - 1);
            EXPECT_EQ(std::accumulate(ALL_CASES_NUM.begin(), ALL_CASES_NUM.begin() + head, 0), archive[0]);
        }
    });
    pool.wait();
}

TEST(ShortCode, code_string) {
    auto test_func = [](ShortCode code) {
        auto code_str = code.to_string();
        EXPECT_EQ(code_str.size(), 5); // length = 5
        for (auto c : code_str) {
            EXPECT_TRUE((c >= '1' && c <= '9') || (c >= 'A' && c <= 'Z'));
            EXPECT_TRUE(c != 'I' && c != 'L' && c != 'O');
        }
        EXPECT_EQ(ShortCode::from_string(code_str), code); // test upper cases
        std::transform(code_str.begin(), code_str.end(), code_str.begin(), ::tolower);
        EXPECT_EQ(ShortCode::from_string(code_str), code); // test lower cases
    };

    BS::thread_pool pool;
    pool.detach_blocks((uint32_t)0, SHORT_CODE_LIMIT, [&test_func](uint32_t start, uint32_t end) {
        for (uint32_t short_code = start; short_code < end; ++short_code) {
            test_func(ShortCode::unsafe_create(short_code));
        }
    });
    pool.wait();
}

TEST(ShortCode, DISABLED_global_verify) {
    all_cases_reset();
    BS::thread_pool pool;
    auto futures = pool.submit_blocks((uint32_t)0, SHORT_CODE_LIMIT, [](uint32_t start, uint32_t end) {
        std::vector<uint64_t> archive;
        archive.reserve(end - start);
        for (uint32_t short_code = start; short_code < end; ++short_code) {
            auto common_code = CommonCode::from_short_code(short_code).value();
            EXPECT_EQ(common_code.to_short_code(), short_code);
            archive.emplace_back(common_code.unwrap());
        }
        return archive;
    }, 0x1000); // split as 4096 pieces

    std::vector<uint64_t> result;
    result.reserve(ALL_CASES_NUM_);
    for (auto &future : futures) {
        const auto data = future.get();
        result.insert(result.end(), data.begin(), data.end()); // combine sections
    }
    pool.wait();
    EXPECT_EQ(result, all_common_codes());
}
