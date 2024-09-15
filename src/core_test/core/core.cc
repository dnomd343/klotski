#include <gtest/gtest.h>

#include "core/core.h"

#include <unordered_set>

#include "utils/common.h"

#include "all_cases/all_cases.h"
#include "common_code/common_code.h"

using klotski::core::Core;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;

using klotski::codec::RawCode;

TEST(core, core) {

    std::vector<uint64_t> raw_codes;
    raw_codes.reserve(klotski::cases::ALL_CASES_NUM_);

    // std::unordered_set<uint64_t> codes;
    // codes.reserve(klotski::cases::ALL_CASES_NUM_);

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = common_code.to_raw_code().unwrap();

            raw_codes.emplace_back(raw_code);
            // codes.emplace(raw_code);
        }
    }

    // auto core = Core([&codes](uint64_t ret, uint64_t) {
    //     EXPECT_EQ(codes.count(ret), 1);
    // });

    std::vector<uint64_t> codes;
    codes.reserve(402258220);

    auto core = Core([&codes](uint64_t ret, uint64_t) {
        codes.emplace_back(klotski::codec::RawCode::unsafe_create(ret).to_common_code().unwrap());
    });

    for (auto raw_code : raw_codes) {
        core.next_cases(raw_code, 0);
    }

    // std::cout << codes.size() << std::endl;

}

// TODO: support multi-thread test

TEST(core, mask) {
    std::vector<uint64_t> raw_codes;
    raw_codes.reserve(klotski::cases::ALL_CASES_NUM_);

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = common_code.to_raw_code().unwrap();
            raw_codes.emplace_back(raw_code);
        }
    }

    uint64_t src;

    auto core = Core([&src](uint64_t ret, uint64_t mask) {

        EXPECT_EQ(std::popcount(mask), 3);

        auto num = std::countr_zero(mask);

        uint64_t full_mask;

        switch ((ret >> num) & 0b111) {
        case BLOCK_1x1:
            full_mask = K_MASK_1x1_ << num;
            break;
        case BLOCK_1x2:
            full_mask = K_MASK_1x2_ << num;
            break;
        case BLOCK_2x1:
            full_mask = K_MASK_2x1_ << num;
            break;
        case BLOCK_2x2:
            full_mask = K_MASK_2x2_ << num;
            break;
        default:
            EXPECT_TRUE(false);
            return;
        }

        auto removed = ret & ~full_mask;

        auto moved = (ret & full_mask) >> num;

        switch ((ret >> num) & 0b111) {
        case BLOCK_1x1:
            EXPECT_EQ(moved, K_MASK_1x1);
            break;
        case BLOCK_1x2:
            EXPECT_EQ(moved, K_MASK_1x2);
            break;
        case BLOCK_2x1:
            EXPECT_EQ(moved, K_MASK_2x1);
            break;
        case BLOCK_2x2:
            EXPECT_EQ(moved, K_MASK_2x2);
            break;
        default:
            EXPECT_TRUE(false);
            return;
        }

//        std::cout << RawCode::unsafe_create(full_mask) << std::endl;
//        std::cout << RawCode::unsafe_create(ret) << std::endl;
//        std::cout << RawCode::unsafe_create(removed) << std::endl;

        auto diff = src ^ removed;
//        std::cout << RawCode::unsafe_create(diff) << std::endl;
//        std::cout << RawCode::unsafe_create(moved) << std::endl;

        int tmp;
        switch ((ret >> num) & 0b111) {
        case BLOCK_1x1:
            tmp = std::countr_zero(diff) - 0;
            EXPECT_EQ(diff >> tmp, moved);
            break;
        case BLOCK_1x2:
            tmp = std::countr_zero(diff) - 0;
            EXPECT_EQ(diff >> tmp, moved);
            break;
        case BLOCK_2x1:
            tmp = std::countr_zero(diff) - 1;
            EXPECT_EQ(diff >> tmp, moved);
            break;
        case BLOCK_2x2:
            tmp = std::countr_zero(diff) - 2;
            EXPECT_EQ(diff >> tmp, moved);
            break;
        default:
            EXPECT_TRUE(false);
            return;
        }

//        std::cout << "----------------" << std::endl;

//        EXPECT_NE(src, ret);

    });

    for (auto raw_code : raw_codes) {

        src = raw_code;

        core.next_cases(raw_code, 0);
    }


//    auto raw_code = RawCode::from_common_code(0x1A9BF0C00).value();
//    auto raw_code = RawCode::from_common_code(0x4FEA13400).value();
//    src = raw_code.unwrap();
//
//    core.next_cases(raw_code.unwrap(), 0);

}
