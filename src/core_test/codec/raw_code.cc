#include <gtest/gtest.h>

#include "test_samples.h"

#include "helper/hash.h"
#include "helper/expect.h"
#include "helper/mirror.h"
#include "helper/parallel.h"

#include "utils/common.h"
#include "raw_code/raw_code.h"
#include "all_cases/all_cases.h"
#include "common_code/common_code.h"

using klotski::range_reverse;

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

static_assert(helper::is_hashable_v<RawCode>);
static_assert(!std::is_default_constructible_v<RawCode>);

static_assert(std::is_trivially_destructible_v<RawCode>);
static_assert(std::is_trivially_copy_assignable_v<RawCode>);
static_assert(std::is_trivially_move_assignable_v<RawCode>);
static_assert(std::is_trivially_copy_constructible_v<RawCode>);
static_assert(std::is_trivially_move_constructible_v<RawCode>);

TEST(RawCode, basic) {
    EXPECT_FALSE(RawCode::check(0x0A34'182B'3810'2D21)); // invalid code
    EXPECT_FALSE(RawCode::check(0x8603'EDF5'CAFF'F5E2)); // high 4-bits not zero

    EXPECT_FALSE(RawCode::unsafe_create(TEST_MIRROR_R1).is_vertical_mirror());
    EXPECT_TRUE(RawCode::unsafe_create(TEST_MIRROR_R1).is_horizontal_mirror());
    EXPECT_EQ(RawCode::unsafe_create(TEST_MIRROR_R1).to_vertical_mirror(), TEST_MIRROR_R1_VM);
    EXPECT_EQ(RawCode::unsafe_create(TEST_MIRROR_R1).to_horizontal_mirror(), TEST_MIRROR_R1_HM);

    EXPECT_FALSE(RawCode::unsafe_create(TEST_MIRROR_R2).is_vertical_mirror());
    EXPECT_FALSE(RawCode::unsafe_create(TEST_MIRROR_R2).is_horizontal_mirror());
    EXPECT_EQ(RawCode::unsafe_create(TEST_MIRROR_R2).to_vertical_mirror(), TEST_MIRROR_R2_VM);
    EXPECT_EQ(RawCode::unsafe_create(TEST_MIRROR_R2).to_horizontal_mirror(), TEST_MIRROR_R2_HM);

#ifndef KLSK_NDEBUG
    EXPECT_OSTREAM(RawCode::unsafe_create(0x3F03C),
                   "00000000003F03C\n@ + . .\n+ + . .\n. . . .\n. . . .\n. . . .\n");
    EXPECT_OSTREAM(RawCode::unsafe_create(TEST_R_CODE),
                   "603EDF5CAFFF5E2\n| @ + |\n+ + + +\n| ~ + |\n+ * * +\n* . . *\n");
#endif
}

TEST(RawCode, exporter) {
    const auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    EXPECT_EQ(raw_code.unwrap(), TEST_R_CODE);
    EXPECT_EQ(raw_code.to_common_code(), TEST_C_CODE);
}

TEST(RawCode, operators) {
    const auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    EXPECT_EQ(static_cast<uint64_t>(raw_code), TEST_R_CODE); // uint64_t cast

    EXPECT_NE(0, raw_code); // uint64_t != RawCode
    EXPECT_NE(raw_code, 0); // RawCode != uint64_t
    EXPECT_EQ(TEST_R_CODE, raw_code); // uint64_t == RawCode
    EXPECT_EQ(raw_code, TEST_R_CODE); // RawCode == uint64_t

    EXPECT_LE(TEST_R_CODE, raw_code); // uint64_t <= RawCode
    EXPECT_LE(TEST_R_CODE - 1, raw_code);
    EXPECT_LT(TEST_R_CODE - 1, raw_code); // uint64_t < RawCode

    EXPECT_LE(raw_code, TEST_R_CODE); // RawCode <= uint64_t
    EXPECT_LE(raw_code, TEST_R_CODE + 1);
    EXPECT_LT(raw_code, TEST_R_CODE + 1); // RawCode < uint64_t

    EXPECT_GE(TEST_R_CODE, raw_code); // uint64_t >= RawCode
    EXPECT_GE(TEST_R_CODE + 1, raw_code);
    EXPECT_GT(TEST_R_CODE + 1, raw_code); // uint64_t > RawCode

    EXPECT_GE(raw_code, TEST_R_CODE); // RawCode >= uint64_t
    EXPECT_GE(raw_code, TEST_R_CODE - 1);
    EXPECT_GT(raw_code, TEST_R_CODE - 1); // RawCode > uint64_t

    EXPECT_EQ(raw_code, raw_code); // RawCode == RawCode
    EXPECT_NE(raw_code, RawCode::unsafe_create(0)); // RawCode != RawCode

    EXPECT_LE(raw_code, raw_code); // RawCode <= RawCode
    EXPECT_LE(raw_code, RawCode::unsafe_create(TEST_R_CODE + 1));
    EXPECT_LT(raw_code, RawCode::unsafe_create(TEST_R_CODE + 1)); // RawCode < RawCode

    EXPECT_GE(raw_code, raw_code); // RawCode >= RawCode
    EXPECT_GE(RawCode::unsafe_create(TEST_R_CODE + 1), raw_code);
    EXPECT_GT(RawCode::unsafe_create(TEST_R_CODE + 1), raw_code); // RawCode > RawCode
}

TEST(RawCode, constexpr) {
    static_assert(RawCode::check(TEST_R_CODE));
    static_assert(!RawCode::check(TEST_R_CODE_ERR));

    static_assert(RawCode::create(TEST_R_CODE).has_value());
    static_assert(!RawCode::create(TEST_R_CODE_ERR).has_value());
    static_assert(RawCode::create(TEST_R_CODE).value() == TEST_R_CODE);

    constexpr auto code = RawCode::unsafe_create(TEST_R_CODE);
    static_assert(static_cast<uint64_t>(code) == TEST_R_CODE);
    static_assert(code.unwrap() == TEST_R_CODE);

    static_assert(code.to_common_code() == TEST_C_CODE);
    static_assert(RawCode(CommonCode::unsafe_create(TEST_C_CODE)) == TEST_R_CODE);
    static_assert(RawCode::from_common_code(TEST_C_CODE).value() == TEST_R_CODE);
    static_assert(RawCode::from_common_code(CommonCode::unsafe_create(TEST_C_CODE)) == TEST_R_CODE);

    constexpr auto mirror_1 = RawCode::unsafe_create(TEST_MIRROR_R1);
    static_assert(!mirror_1.is_vertical_mirror());
    static_assert(mirror_1.is_horizontal_mirror());
    static_assert(mirror_1.to_vertical_mirror() == TEST_MIRROR_R1_VM);
    static_assert(mirror_1.to_horizontal_mirror() == TEST_MIRROR_R1_HM);

    constexpr auto mirror_2 = RawCode::unsafe_create(TEST_MIRROR_R2);
    static_assert(!mirror_2.is_vertical_mirror());
    static_assert(!mirror_2.is_horizontal_mirror());
    static_assert(mirror_2.to_vertical_mirror() == TEST_MIRROR_R2_VM);
    static_assert(mirror_2.to_horizontal_mirror() == TEST_MIRROR_R2_HM);
}

TEST(RawCode, initialize) {
    const auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    const auto common_code = CommonCode::unsafe_create(TEST_C_CODE);

    // operator=
    const auto r1 = raw_code;
    const auto r2 = RawCode {raw_code};
    EXPECT_EQ(r1, TEST_R_CODE); // l-value
    EXPECT_EQ(r2, TEST_R_CODE); // r-value

    // RawCode(...)
    EXPECT_EQ(RawCode(common_code), TEST_R_CODE);
    EXPECT_EQ(RawCode(raw_code), TEST_R_CODE); // l-value
    EXPECT_EQ(RawCode(RawCode(raw_code)), TEST_R_CODE); // r-value

    // RawCode::create(uint64_t)
    EXPECT_TRUE(RawCode::create(TEST_R_CODE).has_value());
    EXPECT_FALSE(RawCode::create(TEST_R_CODE_ERR).has_value());
    EXPECT_EQ(RawCode::create(TEST_R_CODE), TEST_R_CODE);

    // RawCode::unsafe_create(uint64_t)
    EXPECT_EQ(RawCode::unsafe_create(TEST_R_CODE), TEST_R_CODE);
    EXPECT_EQ(RawCode::unsafe_create(TEST_R_CODE_ERR), TEST_R_CODE_ERR);

    // RawCode::from_common_code(CommonCode)
    EXPECT_EQ(RawCode::from_common_code(common_code), TEST_R_CODE);

    // RawCode::from_common_code(uint64_t)
    EXPECT_TRUE(RawCode::from_common_code(TEST_C_CODE).has_value());
    EXPECT_FALSE(RawCode::from_common_code(TEST_C_CODE_ERR).has_value());
    EXPECT_EQ(RawCode::from_common_code(TEST_C_CODE), TEST_R_CODE);

    // RawCode::from_common_code(std::string_view)
    EXPECT_TRUE(RawCode::from_common_code(TEST_C_CODE_STR).has_value());
    EXPECT_FALSE(RawCode::from_common_code(TEST_C_CODE_STR_ERR).has_value());
    EXPECT_EQ(RawCode::from_common_code(TEST_C_CODE_STR), TEST_R_CODE);
}

TEST(RawCode, code_verify) {
    RAW_CODE_PARALLEL({
        EXPECT_TRUE(RawCode::check(code.unwrap()));
        const auto common_code = code.to_common_code(); // RawCode::compact
        EXPECT_EQ(RawCode::from_common_code(common_code), code); // RawCode::extract
    });
}

TEST(RawCode, code_mirror) {
    RAW_CODE_PARALLEL({
        const auto mirror_v = code.to_vertical_mirror();
        EXPECT_EQ(mirror_v, helper::get_vertical_mirror(code));
        EXPECT_EQ(mirror_v.to_vertical_mirror(), code);
        if (mirror_v == code) {
            EXPECT_TRUE(code.is_vertical_mirror());
        } else {
            EXPECT_FALSE(code.is_vertical_mirror());
        }
    });

    RAW_CODE_PARALLEL({
        const auto mirror_h = code.to_horizontal_mirror();
        EXPECT_EQ(mirror_h, helper::get_horizontal_mirror(code));
        EXPECT_EQ(mirror_h.to_horizontal_mirror(), code);
        if (mirror_h == code) {
            EXPECT_TRUE(code.is_horizontal_mirror());
        } else {
            EXPECT_FALSE(code.is_horizontal_mirror());
        }
    });
}

TEST(RawCode, DISABLED_global_verify) {
    // convert CommonCode to RawCode and ignore errors
    static const auto force_convert = +[](const uint64_t common_code) -> uint64_t {
        auto range = range_reverse(static_cast<uint32_t>(common_code));
        auto raw_code = K_MASK_2x2 << (common_code >> 32) * 3; // block 2x2
        for (int addr = 0; range; range >>= 2) {
            while ((raw_code >> addr) & 0b111 && addr < 60) // found next space
                addr += 3;
            if (addr >= 60) // invalid address
                return 0;
            switch (range & 0b11) {
                case 0b01: raw_code |= K_MASK_1x2 << addr; addr += 6; continue;
                case 0b10: raw_code |= K_MASK_2x1 << addr; addr += 3; continue;
                case 0b11: raw_code |= K_MASK_1x1 << addr; addr += 3; continue;
                case 0b00: addr += 3; continue;
                default: std::unreachable();
            }
        }
        return raw_code;
    };

    const auto result = SCOPE_PARALLEL(0x10'0000'0000ULL, [](const uint64_t start, const uint64_t end) {
        std::vector<CommonCode> codes;
        for (uint64_t common_code = start; common_code < end; ++common_code) {
            if (RawCode::check(force_convert(common_code))) {
                codes.emplace_back(CommonCode::unsafe_create(common_code)); // store valid code
            }
        }
        return codes;
    });
    EXPECT_EQ(result, AllCases::instance().fetch().codes());
}
