#include <gtest/gtest.h>

#include "group/group.h"
#include "ranges/ranges.h"

#include "helper/hash.h"
#include "helper/expect.h"

using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::cases::RangesUnion;

using klotski::group::BLOCK_NUM;
using klotski::cases::ALL_CASES_NUM_;

constexpr auto Heads = RangesUnion::Heads;
constexpr std::string_view ALL_CASES_MD5 = "3888e9fab8d3cbb50908b12b147cfb23";

static_assert(std::is_base_of_v<std::array<Ranges, 16>, RangesUnion>);

TEST(RangesUnion, heads) {
    for (const auto head : RangesUnion::Heads) {
        EXPECT_GE(head, 0);
        EXPECT_LT(head, 16);
        EXPECT_NE(head % 4, 3);
    }
    EXPECT_SORTED_AND_UNIQUE(RangesUnion::Heads);
}

TEST(RangesUnion, index) {
    const auto &all_cases = AllCases::instance().fetch();
    const auto codes = all_cases.codes();
    for (size_t i = 0; i < codes.size(); ++i) {
        EXPECT_EQ(all_cases[i], codes[i]); // test `operator[]`
    }
    EXPECT_EQ(all_cases.size(), ALL_CASES_NUM_);
}

TEST(RangesUnion, export) {
    std::string buffer;
    buffer.reserve(ALL_CASES_NUM_ * 10); // [\dA-F]{9} + '\n'
    for (const auto code : AllCases::instance().fetch().codes()) {
        buffer += std::format("{:09X}\n", code.unwrap());
    }
    EXPECT_EQ(helper::md5(buffer), ALL_CASES_MD5);
}

TEST(RangesUnion, ranges) {
    auto mut_cases = AllCases::instance().fetch();
    for (const auto head : Heads) {
        auto &ranges = mut_cases.std::array<Ranges, 16>::operator[](head);
        EXPECT_EQ(mut_cases.ranges(head), ranges);
    }

    const auto const_cases = AllCases::instance().fetch();
    for (const auto head : Heads) {
        const auto &ranges = const_cases.std::array<Ranges, 16>::operator[](head);
        EXPECT_EQ(const_cases.ranges(head), ranges);
    }
}

TEST(RangesUnion, append) {
    RangesUnion cases;
    for (const auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges ranges;
        ranges.spawn(n, n_2x1, n_1x1);
        ranges.reverse();

        RangesUnion ranges_union;
        for (const auto head : Heads) {
            ranges.derive(static_cast<int>(head), ranges_union.ranges(head));
        }
        auto &tmp = cases += ranges_union;
        EXPECT_EQ(tmp, cases); // reference of cases
    }

    for (const auto head : Heads) {
        std::ranges::stable_sort(cases.ranges(head).begin(), cases.ranges(head).end());
    }
    EXPECT_EQ(cases, AllCases::instance().fetch());
}
