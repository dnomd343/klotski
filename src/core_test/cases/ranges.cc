#include <gtest/gtest.h>

#include "helper/cases.h"
#include "helper/expect.h"
#include "helper/block_num.h"

#include "group/group.h"
#include "ranges/ranges.h"

using klotski::range_reverse;

static_assert(std::is_base_of_v<std::vector<uint32_t>, Ranges>);

TEST(Ranges, check) {
    RangesUnion all_cases;
    for (const auto head : Heads) {
        for (auto range : BasicRanges::instance().fetch()) {
            if (Ranges::check(head, range_reverse(range)) == 0) {
                all_cases.ranges(head).emplace_back(range); // found valid cases
            }
        }
    }
    EXPECT_EQ(all_cases, AllCases::instance().fetch());
}

TEST(Ranges, spawn) {
    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        EXPECT_SORTED_AND_UNIQUE(ranges); // sorted and unique
        for (const auto range : ranges) {
            const auto [val_1x1, val_1x2, val_2x1] = helper::cal_block_num(range);
            EXPECT_EQ(val_1x1, n_1x1);
            EXPECT_EQ(val_2x1, n_2x1);
            EXPECT_EQ(val_1x2 + val_2x1, n); // verify block nums
        }
    }
}

TEST(Ranges, derive) {
    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        ranges.reverse(); // reverse ranges for derive

        RangesUnion cases;
        for (const auto head : Heads) {
            ranges.derive(head, cases.ranges(head));
            EXPECT_SORTED_AND_UNIQUE(cases.ranges(head)); // sorted and unique
            EXPECT_COMMON_CODES(head, cases.ranges(head)); // verify common codes
        }

        ranges.reverse();
        for (const auto head : Heads) {
            EXPECT_SUBSET(ranges, cases.ranges(head)); // derive ranges is subset
        }
    }
}

TEST(Ranges, append) {
    Ranges ranges;
    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges r;
        r.spawn(n, n_2x1, n_1x1);
        auto &tmp = ranges += r;
        EXPECT_EQ(tmp, ranges); // reference of ranges
    }
    std::stable_sort(ranges.begin(), ranges.end());
    EXPECT_EQ(ranges, BasicRanges::instance().fetch());
}

TEST(Ranges, reverse) {
    auto ranges = BasicRanges::instance().fetch();
    auto reverse = ranges | std::views::transform(range_reverse) | std::ranges::to<std::vector>();
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
        for (const auto head : Heads) {
            ranges.derive(head, all_cases.ranges(head)); // derive from sub ranges
        }
    }

    std::ranges::stable_sort(all_ranges.begin(), all_ranges.end());
    for (const auto head : Heads) {
        std::ranges::stable_sort(all_cases.ranges(head).begin(), all_cases.ranges(head).end());
    }
    EXPECT_EQ(all_ranges, BasicRanges::instance().fetch()); // verify all ranges
    EXPECT_EQ(all_cases, AllCases::instance().fetch()); // verify all cases

    all_ranges.reverse(); // reverse ranges for derive
    for (const auto head : Heads) {
        all_cases.ranges(head).clear();
        all_ranges.derive(head, all_cases.ranges(head)); // derive from all ranges
    }
    EXPECT_EQ(all_cases, AllCases::instance().fetch()); // verify content
}
