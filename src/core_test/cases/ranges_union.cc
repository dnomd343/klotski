#include <gtest/gtest.h>

#include "helper/cases.h"
#include "ranges/ranges.h"

static_assert(std::is_base_of_v<std::array<Ranges, 16>, RangesUnion>);

// TODO: test CommonCode export

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
