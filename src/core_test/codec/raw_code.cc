#include <gtest/gtest.h>
#include <BS_thread_pool.hpp>

#include "utils/common.h"
#include "helper/codec.h"
#include "helper/sample.h"
#include "raw_code/raw_code.h"
#include "all_cases/all_cases.h"
#include "common_code/common_code.h"

using klotski::range_reverse;

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::cases::AllCases;
using klotski::cases::ALL_CASES_NUM_;

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
    std::ostringstream out;
    out << RawCode::unsafe_create(TEST_R_CODE); // ostream capture
    EXPECT_EQ(out.str(), "603EDF5CAFFF5E2\n| @ + |\n+ + + +\n| ~ + |\n+ * * +\n* . . *\n");
#endif
}

TEST(RawCode, exporter) {
    auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    EXPECT_EQ(raw_code.unwrap(), TEST_R_CODE);
    EXPECT_EQ(raw_code.to_common_code(), TEST_C_CODE);
}

TEST(RawCode, operators) {
    auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    EXPECT_EQ((uint64_t)raw_code, TEST_R_CODE); // uint64_t cast

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

TEST(RawCode, initialize) {
    auto raw_code = RawCode::unsafe_create(TEST_R_CODE);
    auto common_code = CommonCode::unsafe_create(TEST_C_CODE);

    // operator=
    auto r1 = raw_code;
    auto r2 = RawCode {raw_code};
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
        EXPECT_TRUE(RawCode::check(mirror_v.unwrap()));
        EXPECT_EQ(mirror_v.to_vertical_mirror(), code);
        EXPECT_FALSE(mirror_v.is_vertical_mirror()); // not exist
        EXPECT_NE(mirror_v, code);

        const auto mirror_h = code.to_horizontal_mirror();
        EXPECT_TRUE(RawCode::check(mirror_h.unwrap()));
        EXPECT_EQ(mirror_h.to_horizontal_mirror(), code);
        if (mirror_h.is_horizontal_mirror()) {
            EXPECT_EQ(mirror_h, code);
        } else {
            EXPECT_NE(mirror_h, code);
        }
    });
}

TEST(RawCode, DISABLED_global_verify) {
    // convert to RawCode and ignore errors
    static auto force_convert = +[](uint64_t common_code) -> uint64_t {
        auto range = range_reverse(static_cast<uint32_t>(common_code));
        auto raw_code = K_MASK_2x2 << (common_code >> 32) * 3;
        for (int addr = 0; range; range >>= 2) {
            while ((raw_code >> addr) & 0b111 && addr < 60) // found next space
                addr += 3;
            if (addr >= 60) // invalid address
                return 0;
            switch (range & 0b11) {
                case 0b01: raw_code |= K_MASK_1x2 << addr; break;
                case 0b10: raw_code |= K_MASK_2x1 << addr; break;
                case 0b11: raw_code |= K_MASK_1x1 << addr; break;
                case 0b00: addr += 3;
            }
        }
        return raw_code;
    };

    const auto result = parallel_spawn(0x10'0000'0000ULL, [](uint64_t start, uint64_t end) {
        std::vector<uint64_t> codes;
        for (uint64_t common_code = start; common_code < end; ++common_code) {
            if (RawCode::check(force_convert(common_code))) {
                codes.emplace_back(common_code); // store valid code
            }
        }
        return codes;
    });
    EXPECT_EQ(result, all_common_codes());
}
