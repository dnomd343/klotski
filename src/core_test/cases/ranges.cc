#include <gtest/gtest.h>

#include "group/group.h"
#include "helper/cases.h"
#include "ranges/ranges.h"

static_assert(std::is_base_of_v<std::vector<uint32_t>, Ranges>);

TEST(Ranges, check) {
    RangesUnion all_cases;
    for (const auto head : Heads) {
        for (auto range : BasicRanges::instance().fetch()) {
            if (Ranges::check(head, range_reverse(range)) == 0) {
                all_cases[head].emplace_back(range); // found valid cases
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
            const auto [val_1x1, val_1x2, val_2x1] = cal_block_num(range);
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
            ranges.derive(head, cases[head]);
            EXPECT_SORTED_AND_UNIQUE(cases[head]); // sorted and unique
            EXPECT_COMMON_CODES(head, cases[head]); // verify common codes
        }

        ranges.reverse();
        for (const auto head : Heads) {
            EXPECT_SUBSET(ranges, cases[head]); // derive ranges is subset
        }
    }
}

TEST(Ranges, reverse) {
    auto ranges = BasicRanges::instance().fetch();
    Ranges reverse {ranges};

    for (auto &x : reverse) {
        x = range_reverse(x); // manual reverse
    }
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
        all_cases[head].reserve(ALL_CASES_NUM[head]); // pre reserve
    }

    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        all_ranges += ranges;
        ranges.reverse(); // reverse ranges for derive
        for (const auto head : Heads) {
            ranges.derive(head, all_cases[head]); // derive from sub ranges
        }
    }

    std::ranges::stable_sort(all_ranges.begin(), all_ranges.end());
    for (const auto head : Heads) {
        std::ranges::stable_sort(all_cases[head].begin(), all_cases[head].end());
    }
    EXPECT_EQ(all_ranges, BasicRanges::instance().fetch()); // verify content
    EXPECT_EQ(all_cases, AllCases::instance().fetch()); // verify content

    all_ranges.reverse(); // reverse ranges for derive
    for (const auto head : Heads) {
        all_cases[head].clear();
        all_ranges.derive(head, all_cases[head]); // derive from all ranges
    }
    EXPECT_EQ(all_cases, AllCases::instance().fetch()); // verify content
}

TEST(Ranges, operator) {
    Ranges r, r1, r2;
    r.spawn(5, 3, 4);
    r.spawn(5, 4, 4);
    r1.spawn(5, 3, 4);
    r2.spawn(5, 4, 4);
    EXPECT_EQ(r, r1 += r2);
}
