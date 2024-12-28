#include <gtest/gtest.h>

#include "group/group.h"
#include "ranges/ranges.h"

#include "helper/expect.h"
#include "helper/parallel.h"
#include "helper/block_num.h"

using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::codec::CommonCode;
using klotski::cases::BasicRanges;
using klotski::cases::RangesUnion;

using klotski::range_reverse;
using klotski::group::BLOCK_NUM;
using klotski::cases::ALL_CASES_NUM;
using klotski::cases::BASIC_RANGES_NUM_;

constexpr auto Heads = RangesUnion::Heads;

static_assert(std::is_default_constructible_v<Ranges>); // TODO: more concept assert
static_assert(std::is_base_of_v<std::vector<uint32_t>, Ranges>);

TEST(Ranges, check) {
    HEAD_PARALLEL({
        Ranges cases;
        cases.reserve(ALL_CASES_NUM[head]);
        for (const auto range : BasicRanges::instance().fetch()) {
            if (Ranges::check(static_cast<int>(head), range_reverse(range)) == 0) {
                cases.emplace_back(range); // found valid cases
            }
        }
        EXPECT_EQ(cases, AllCases::instance().fetch().ranges(head));
    });
}

TEST(Ranges, spawn) {
    BLOCK_NUM_PARALLEL({
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        EXPECT_SORTED_AND_UNIQUE(ranges); // sorted and unique

        for (const auto range : ranges) {
            const auto val = helper::cal_block_num(range);
            EXPECT_EQ(n_1x1, val.n_1x1);
            EXPECT_EQ(n_2x1, val.n_2x1);
            EXPECT_EQ(n, val.n_1x2 + val.n_2x1); // verify block nums
        }
    });
}

TEST(Ranges, derive) {
    BLOCK_NUM_PARALLEL({
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        ranges.reverse(); // reverse ranges for derive

        RangesUnion cases;
        for (const auto head : Heads) {
            ranges.derive(static_cast<int>(head), cases.ranges(head));
            EXPECT_SORTED_AND_UNIQUE(cases.ranges(head)); // sorted and unique
            EXPECT_COMMON_CODES(head, cases.ranges(head)); // verify common codes
        }

        ranges.reverse();
        for (const auto head : Heads) {
            EXPECT_SUBSET(ranges, cases.ranges(head)); // derive ranges is subset
        }
    });
}

TEST(Ranges, append) {
    Ranges ranges;
    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges r;
        r.spawn(n, n_2x1, n_1x1);
        auto &tmp = ranges += r;
        EXPECT_EQ(tmp, ranges); // reference of ranges
    }
    std::ranges::stable_sort(ranges.begin(), ranges.end());
    EXPECT_EQ(ranges, BasicRanges::instance().fetch());
}

TEST(Ranges, reverse) {
    auto ranges = BasicRanges::instance().fetch();
    const auto reverse = ranges
        | std::views::transform(range_reverse)
        | std::ranges::to<std::vector>();
    ranges.reverse();
    EXPECT_EQ(ranges, reverse);
    ranges.reverse();
    EXPECT_EQ(ranges, BasicRanges::instance().fetch());
}

TEST(Ranges, combine) {
    Ranges all_ranges;
    RangesUnion all_cases;
    all_ranges.reserve(BASIC_RANGES_NUM_); // pre reserve
    for (const auto head : Heads) {
        all_cases.ranges(head).reserve(ALL_CASES_NUM[head]); // pre reserve
    }

    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        all_ranges += ranges;
        ranges.reverse(); // reverse ranges for derive
        for (const auto head : Heads) { // derive from sub ranges
            ranges.derive(static_cast<int>(head), all_cases.ranges(head));
        }
    }

    std::ranges::stable_sort(all_ranges.begin(), all_ranges.end());
    for (const auto head : Heads) {
        std::ranges::stable_sort(all_cases.ranges(head).begin(), all_cases.ranges(head).end());
    }
    EXPECT_EQ(all_ranges, BasicRanges::instance().fetch()); // verify all ranges
    EXPECT_EQ(all_cases, AllCases::instance().fetch()); // verify all cases

    all_ranges.reverse(); // reverse ranges for derive
    for (const auto head : Heads) { // derive from all ranges
        all_cases.ranges(head).clear();
        all_ranges.derive(static_cast<int>(head), all_cases.ranges(head));
    }
    EXPECT_EQ(all_cases, AllCases::instance().fetch()); // verify content
}

TEST(Ranges, constexpr) {
    static_assert(Ranges::check(1, 0x0030FE6A) == 0);
    static_assert(Ranges::check(4, 0x001C4ABF) == 0);

    static_assert(Ranges::check(1, 0xA9BF0C00) == 14);
    static_assert(Ranges::check(4, 0xFEA13400) == 6);
}
