#include <gtest/gtest.h>

#include <format>

#include "hash.h"
#include "helper/cases.h"
#include "ranges/ranges.h"

static_assert(std::is_base_of_v<std::array<Ranges, 16>, RangesUnion>);

constexpr std::string_view ALL_CASES_MD5 = "3888e9fab8d3cbb50908b12b147cfb23";

TEST(RangesUnion, export) {
    std::string buffer;
    for (auto code : AllCases::instance().fetch().codes()) {
        buffer += std::format("{:09X}\n", code.unwrap());
    }
    EXPECT_EQ(hash::md5(buffer), ALL_CASES_MD5);
}

TEST(RangesUnion, operator) {
    Ranges r, r1, r2;
    r.spawn(5, 3, 4);
    r.spawn(5, 4, 4);
    r1.spawn(5, 3, 4);
    r2.spawn(5, 4, 4);

    RangesUnion r_, r1_, r2_;
    for (const auto head : Heads) {
        r.derive(head, r_[head]);
        r1.derive(head, r1_[head]);
        r2.derive(head, r2_[head]);
    }
    EXPECT_EQ(r_, r1_ += r2_);
}
