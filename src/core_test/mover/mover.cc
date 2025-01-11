#include <gtest/gtest.h>

#include <algorithm>
#include <unordered_set>
#include <numeric>
#include <ranges>

#include "mover/mover.h"
#include "utils/common.h"
#include "all_cases/all_cases.h"
#include "common_code/common_code.h"

#include "helper/hash.h"

constexpr auto NEXT_CASES_XXH3 = std::to_array<uint64_t>({
    0xcd1920b50bc3bda1, 0xd881004a12384988, 0xbdefaaee9508848d, 0x2d06800538d394c2,
    0xb1c484293104fbfc, 0x0474e965bfd0bf98, 0x76891a6b2906d7d6, 0x2d06800538d394c2,
    0x878de6f355826c4d, 0xe67c680ab0cbfd21, 0x0b987953e6860717, 0x2d06800538d394c2,
    0x4a7599e1bdbffbb3, 0xb3cf1fdea988466a, 0x21226a4f692e1892, 0x2d06800538d394c2,
});

using klotski::mover::MaskMover;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;

using klotski::codec::RawCode;

// TODO: test input mask with samples

TEST(Mover, core) {

    // TODO: test for each type_id

    std::vector<uint64_t> raw_codes;
    raw_codes.reserve(klotski::cases::ALL_CASES_NUM_);

    // std::unordered_set<uint64_t> codes;
    // codes.reserve(klotski::cases::ALL_CASES_NUM_);

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch().ranges(head)) {
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

    auto core = MaskMover([&codes](RawCode ret, uint64_t) {
        codes.emplace_back(ret.to_common_code().unwrap());
    });

    for (auto raw_code : raw_codes) {
        core.next_cases(RawCode::unsafe_create(raw_code), 0);
    }

    // std::cout << codes.size() << std::endl;

}

// TODO: support multi-thread test

TEST(Mover, mask) {
    std::vector<uint64_t> raw_codes;
    raw_codes.reserve(klotski::cases::ALL_CASES_NUM_);

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch().ranges(head)) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = common_code.to_raw_code().unwrap();
            raw_codes.emplace_back(raw_code);
        }
    }

    uint64_t src;

    auto core = MaskMover([&src](RawCode ret_, uint64_t mask) {
        const uint64_t ret = ret_.unwrap();

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

        auto diff = src ^ removed;

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

    });

    for (auto raw_code : raw_codes) {

        src = raw_code;

        core.next_cases(RawCode::unsafe_create(raw_code), 0);
    }


}

TEST(Mover, next_cases) {

    std::vector<RawCode> raw_codes;

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch().ranges(head)) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);

            auto raw_code = common_code.to_raw_code();
            raw_codes.emplace_back(raw_code);
        }
        break;
    }

    std::vector<CommonCode> result {};

    std::vector<CommonCode> tmp;
    auto core = MaskMover([&tmp](RawCode ret, uint64_t mask) {
        tmp.emplace_back(CommonCode::from_raw_code(ret));
    });

    for (const auto raw_code : raw_codes) {

        result.emplace_back(raw_code.to_common_code());
//        std::cout << raw_code.to_common_code() << "->";
        core.next_cases(raw_code, 0);

        std::stable_sort(tmp.begin(), tmp.end());
        for (uint64_t i = 0; i < tmp.size(); ++i) {
            result.emplace_back(tmp[i]);
//            std::cout << tmp[i];
//            if (i + 1 != tmp.size()) {
//                std::cout << "/";
//            }
        }

        tmp.clear();
//        std::cout << std::endl;

    }

//    std::cout << result.size() << std::endl;

//    std::cout << std::hex << hash::xxh3(result) << std::endl;

    EXPECT_EQ(helper::xxh3(result), NEXT_CASES_XXH3[0]);

}
