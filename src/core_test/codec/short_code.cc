#include <algorithm>
#include <gtest/gtest.h>

#include "test_samples.h"
#include "helper/expect.h"
#include "helper/parallel.h"
#include "utility/exposer.h"
#include "helper/concurrent.h"

#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::cases::BasicRanges;
using klotski::cases::RangesUnion;

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::codec::SHORT_CODE_LIMIT;

/// Forcibly modify private variables to reset state.
EXPOSE_VAR(AllCases, bool, available_)
EXPOSE_VAR(BasicRanges, bool, available_)
EXPOSE_STATIC_VAR(ShortCode, bool, fast_)
EXPOSE_STATIC_VAR(ShortCode, const RangesUnion*, cases_)
EXPOSE_STATIC_VAR(ShortCode, std::atomic<const Ranges*>, ranges_)

/// Reset ShortCode speed up state, note it is thread-unsafe.
void speed_up_reset() {
    exposer::ShortCode_fast_() = false;
    exposer::ShortCode_cases_() = nullptr;
    exposer::ShortCode_ranges_() = nullptr;
    exposer::AllCases_available_(AllCases::instance()) = false;
    exposer::BasicRanges_available_(BasicRanges::instance()) = false;
}

TEST(ShortCode, basic) {
    EXPECT_FALSE(ShortCode::check(-1)); // out of short code range
    EXPECT_FALSE(ShortCode::check(29670987)); // out of short code range

    EXPECT_FALSE(ShortCode::from_string("R50EH").has_value()); // with invalid `0`
    EXPECT_FALSE(ShortCode::from_string("123456").has_value()); // length != 5
    EXPECT_FALSE(ShortCode::from_string("Z9EFV").has_value()); // out of short code range

    const auto sum = std::accumulate(ALL_CASES_NUM.begin(), ALL_CASES_NUM.end(), 0);
    EXPECT_EQ(sum, SHORT_CODE_LIMIT);

#ifndef KLSK_NDEBUG
    EXPECT_OSTREAM(ShortCode::unsafe_create(0), "11111");
    EXPECT_OSTREAM(ShortCode::unsafe_create(TEST_S_CODE), TEST_S_CODE_STR);
#endif
}

TEST(ShortCode, operators) {
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);
    EXPECT_EQ(static_cast<uint32_t>(short_code), TEST_S_CODE); // uint32_t cast

    EXPECT_NE(0, short_code); // uint32_t != ShortCode
    EXPECT_NE(short_code, 0); // ShortCode != uint32_t
    EXPECT_EQ(TEST_S_CODE, short_code); // uint32_t == ShortCode
    EXPECT_EQ(short_code, TEST_S_CODE); // ShortCode == uint32_t

    EXPECT_LE(TEST_S_CODE, short_code); // uint32_t <= ShortCode
    EXPECT_LE(TEST_S_CODE - 1, short_code);
    EXPECT_LT(TEST_S_CODE - 1, short_code); // uint32_t < ShortCode

    EXPECT_LE(short_code, TEST_S_CODE); // ShortCode <= uint32_t
    EXPECT_LE(short_code, TEST_S_CODE + 1);
    EXPECT_LT(short_code, TEST_S_CODE + 1); // ShortCode < uint32_t

    EXPECT_GE(TEST_S_CODE, short_code); // uint32_t >= ShortCode
    EXPECT_GE(TEST_S_CODE + 1, short_code);
    EXPECT_GT(TEST_S_CODE + 1, short_code); // uint32_t > ShortCode

    EXPECT_GE(short_code, TEST_S_CODE); // ShortCode >= uint32_t
    EXPECT_GE(short_code, TEST_S_CODE - 1);
    EXPECT_GT(short_code, TEST_S_CODE - 1); // ShortCode > uint32_t

    EXPECT_EQ(short_code, short_code); // ShortCode == ShortCode
    EXPECT_NE(short_code, ShortCode::unsafe_create(0)); // ShortCode != ShortCode

    EXPECT_LE(short_code, short_code); // ShortCode <= ShortCode
    EXPECT_LE(short_code, ShortCode::unsafe_create(TEST_S_CODE + 1));
    EXPECT_LT(short_code, ShortCode::unsafe_create(TEST_S_CODE + 1)); // ShortCode < ShortCode

    EXPECT_GE(short_code, short_code); // ShortCode >= ShortCode
    EXPECT_GE(ShortCode::unsafe_create(TEST_S_CODE + 1), short_code);
    EXPECT_GT(ShortCode::unsafe_create(TEST_S_CODE + 1), short_code); // ShortCode > ShortCode
}

TEST(ShortCode, exporter) {
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);
    EXPECT_EQ(short_code.unwrap(), TEST_S_CODE);
    EXPECT_EQ(short_code.to_string(), TEST_S_CODE_STR);

    speed_up_reset();
    EXPECT_EQ(short_code.to_common_code(), TEST_C_CODE);
    ShortCode::speed_up(true);
    EXPECT_EQ(short_code.to_common_code(), TEST_C_CODE);
}

TEST(ShortCode, initialize) {
    auto short_code = ShortCode::unsafe_create(TEST_S_CODE);
    auto common_code = CommonCode::unsafe_create(TEST_C_CODE);

    // operator=
    auto s1 = short_code;
    auto s2 = ShortCode {short_code};
    EXPECT_EQ(s1, TEST_S_CODE); // l-value
    EXPECT_EQ(s2, TEST_S_CODE); // r-value

    // ShortCode(...)
    EXPECT_EQ(ShortCode(common_code), TEST_S_CODE);
    EXPECT_EQ(ShortCode(short_code), TEST_S_CODE); // l-value
    EXPECT_EQ(ShortCode(ShortCode(short_code)), TEST_S_CODE); // r-value

    // ShortCode::create(uint32_t)
    speed_up_reset();
    EXPECT_TRUE(ShortCode::create(TEST_S_CODE).has_value());
    EXPECT_FALSE(ShortCode::create(TEST_S_CODE_ERR).has_value());
    EXPECT_EQ(ShortCode::create(TEST_S_CODE), TEST_S_CODE);
    ShortCode::speed_up(true);
    EXPECT_TRUE(ShortCode::create(TEST_S_CODE).has_value());
    EXPECT_FALSE(ShortCode::create(TEST_S_CODE_ERR).has_value());
    EXPECT_EQ(ShortCode::create(TEST_S_CODE), TEST_S_CODE);

    // ShortCode::unsafe_create(uint32_t)
    EXPECT_EQ(ShortCode::unsafe_create(TEST_S_CODE), TEST_S_CODE);

    // ShortCode::from_string(std::string_view)
    EXPECT_TRUE(ShortCode::from_string(TEST_S_CODE_STR).has_value());
    EXPECT_FALSE(ShortCode::from_string(TEST_S_CODE_STR_ERR).has_value());
    EXPECT_EQ(ShortCode::from_string(TEST_S_CODE_STR), TEST_S_CODE);

    // ShortCode::from_common_code(CommonCode)
    EXPECT_EQ(ShortCode::from_common_code(common_code), TEST_S_CODE);

    // ShortCode::from_common_code(uint64_t)
    EXPECT_TRUE(ShortCode::from_common_code(TEST_C_CODE).has_value());
    EXPECT_FALSE(ShortCode::from_common_code(TEST_C_CODE_ERR).has_value());
    EXPECT_EQ(ShortCode::from_common_code(TEST_C_CODE), TEST_S_CODE);

    // ShortCode::from_common_code(std::string_view)
    EXPECT_TRUE(ShortCode::from_common_code(TEST_C_CODE_STR).has_value());
    EXPECT_FALSE(ShortCode::from_common_code(TEST_C_CODE_STR_ERR).has_value());
    EXPECT_EQ(ShortCode::from_common_code(TEST_C_CODE_STR), TEST_S_CODE);
}

TEST(ShortCode, speed_up) {
    helper::Racer racer {};

    static auto EXPECT_STAGE_0 = +[]() {
        EXPECT_FALSE(exposer::ShortCode_fast_());
        EXPECT_EQ(exposer::ShortCode_cases_(), nullptr);
        EXPECT_EQ(exposer::ShortCode_ranges_(), nullptr);
        EXPECT_FALSE(BasicRanges::instance().is_available());
        EXPECT_FALSE(AllCases::instance().is_available());
    };

    static auto EXPECT_STAGE_1 = +[]() {
        EXPECT_FALSE(exposer::ShortCode_fast_());
        EXPECT_EQ(exposer::ShortCode_cases_(), nullptr);
        EXPECT_EQ(exposer::ShortCode_ranges_(), &BasicRanges::instance().fetch());
        EXPECT_TRUE(BasicRanges::instance().is_available());
        EXPECT_FALSE(AllCases::instance().is_available());
    };

    static auto EXPECT_STAGE_2 = +[]() {
        EXPECT_TRUE(exposer::ShortCode_fast_());
        EXPECT_EQ(exposer::ShortCode_cases_(), &AllCases::instance().fetch());
        EXPECT_EQ(exposer::ShortCode_ranges_(), &BasicRanges::instance().fetch());
        EXPECT_TRUE(BasicRanges::instance().is_available());
        EXPECT_TRUE(AllCases::instance().is_available());
    };

    speed_up_reset();
    EXPECT_STAGE_0();
    racer.Execute([] { ShortCode::speed_up(false); });
    EXPECT_STAGE_1();
    racer.Execute([] { ShortCode::speed_up(true); });
    EXPECT_STAGE_2();
    racer.Execute([] { ShortCode::speed_up(true); });
    EXPECT_STAGE_2();
    racer.Execute([] { ShortCode::speed_up(false); });
    EXPECT_STAGE_2();

    speed_up_reset();
    EXPECT_STAGE_0();
    racer.Execute([] { ShortCode::speed_up(true); });
    EXPECT_STAGE_2();
}

TEST(ShortCode, code_verify) {
    ShortCode::speed_up(true); // enter fast mode
    SHORT_CODE_PARALLEL({
        EXPECT_TRUE(ShortCode::check(code.unwrap()));
        auto common_code = code.to_common_code(); // ShortCode::fast_decode
        EXPECT_EQ(ShortCode::from_common_code(common_code), code); // ShortCode::fast_encode
    });
}

TEST(ShortCode, code_string) {
    SHORT_CODE_PARALLEL({
        auto code_str = code.to_string();
        EXPECT_EQ(code_str.size(), 5); // length = 5
        for (auto c : code_str) {
            EXPECT_TRUE((c >= '1' && c <= '9') || (c >= 'A' && c <= 'Z'));
            EXPECT_TRUE(c != 'I' && c != 'L' && c != 'O');
        }
        EXPECT_EQ(ShortCode::from_string(code_str), code); // test upper cases
        std::transform(code_str.begin(), code_str.end(), code_str.begin(), ::tolower);
        EXPECT_EQ(ShortCode::from_string(code_str), code); // test lower cases
    });
}

TEST(ShortCode, DISABLED_global_verify) {
    speed_up_reset();
    const auto result = SCOPE_PARALLEL(SHORT_CODE_LIMIT, [](uint32_t start, uint32_t end) {
        std::vector<CommonCode> codes;
        codes.reserve(end - start);
        for (uint32_t short_code = start; short_code < end; ++short_code) {
            auto common_code = CommonCode::from_short_code(short_code).value(); // ShortCode::tiny_decode
            EXPECT_EQ(common_code.to_short_code(), short_code); // ShortCode::tiny_encode
            codes.emplace_back(common_code);
        }
        return codes;
    });
    EXPECT_EQ(result, AllCases::instance().fetch().codes());
}
