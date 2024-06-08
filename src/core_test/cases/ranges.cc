#include <gtest/gtest.h>

#include "ranges/ranges.h"

#include <algorithm>
#include <group/group.h>

using klotski::cases::Ranges;

static_assert(std::is_base_of_v<std::vector<uint32_t>, Ranges>);
static_assert(std::is_base_of_v<std::array<Ranges, 16>, klotski::cases::RangesUnion>);

TEST(Ranges, demo) {

    for (int type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto [n, n_2x1, n_1x1] = klotski::cases::BLOCK_NUM[type_id];

        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);

        EXPECT_TRUE(std::ranges::is_sorted(ranges.begin(), ranges.end()));

        const auto match = std::ranges::adjacent_find(ranges.begin(), ranges.end());

        EXPECT_EQ(match, ranges.end());

    }

}

TEST(Ranges, combine) {

    Ranges ranges;

    for (int type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto [n, n_2x1, n_1x1] = klotski::cases::BLOCK_NUM[type_id];

        ranges.spawn(n, n_2x1, n_1x1);

    }

    std::ranges::stable_sort(ranges.begin(), ranges.end());

    EXPECT_EQ(ranges, klotski::cases::BasicRanges::instance().fetch());

}

TEST(Ranges, reverse) {

    for (int type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto [n, n_2x1, n_1x1] = klotski::cases::BLOCK_NUM[type_id];

        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);

        Ranges reverse {ranges};
        for (auto &x : reverse) {
            x = klotski::range_reverse(x);
        }
        ranges.reverse();

        EXPECT_EQ(ranges, reverse);

    }


}

TEST(Ranges, derive) {

    for (int type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {
        auto [n, n_2x1, n_1x1] = klotski::cases::BLOCK_NUM[type_id];

        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);

        ranges.reverse();

        auto kk = klotski::cases::GroupUnion::unsafe_create(type_id);

        klotski::cases::RangesUnion result;

        for (int head = 0; head < 16; ++head) {

            if (head % 4 == 3) {
                continue;
            }

            ranges.derive(head, result[head]);
        }

        EXPECT_EQ(result, kk.cases());

    }


}

TEST(Ranges, check) {
    auto ranges = klotski::cases::BasicRanges::instance().fetch();

    klotski::cases::RangesUnion all_cases;

    for (int head = 0; head < 16; ++head) {
        if (head % 4 == 3) {
            continue;
        }

        for (auto range : ranges) {
            if (Ranges::check(head, klotski::range_reverse(range)) == 0) {
                all_cases[head].emplace_back(range);
            }
        }
    }

    EXPECT_EQ(klotski::cases::AllCases::instance().fetch(), all_cases);

}

// TODO: export CommonCode
