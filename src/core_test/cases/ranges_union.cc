#include <gtest/gtest.h>

#include "helper/hash.h"
#include "helper/cases.h"
#include "helper/expect.h"
#include "ranges/ranges.h"

constexpr std::string_view ALL_CASES_MD5 = "3888e9fab8d3cbb50908b12b147cfb23";

static_assert(std::is_base_of_v<std::array<Ranges, 16>, RangesUnion>);

TEST(RangesUnion, export) {
    std::string buffer;
    buffer.reserve(ALL_CASES_NUM_ * 10); // [\dA-F]{9} + '\n'
    for (auto code : AllCases::instance().fetch().codes()) {
        buffer += std::format("{:09X}\n", code.unwrap());
    }
    EXPECT_EQ(helper::md5(buffer), ALL_CASES_MD5);
}

TEST(RangesUnion, append) {
    RangesUnion cases;
    for (auto [n, n_2x1, n_1x1] : BLOCK_NUM) {
        Ranges r;
        r.spawn(n, n_2x1, n_1x1);
        r.reverse();

        RangesUnion ru;
        for (const auto head : Heads) {
            r.derive(head, ru.ranges(head));
        }
        auto &tmp = cases += ru;
        EXPECT_EQ(tmp, cases); // reference of cases
    }

    for (const auto head : Heads) {
        std::stable_sort(cases.ranges(head).begin(), cases.ranges(head).end());
    }
    EXPECT_EQ(cases, AllCases::instance().fetch());
}
