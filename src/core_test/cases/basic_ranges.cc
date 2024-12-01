#include <gtest/gtest.h>
#include <utility/exposer.h>

#include "group/group.h"
#include "ranges/ranges.h"
#include "all_cases/all_cases.h"

#include "helper/hash.h"
#include "helper/expect.h"
#include "helper/fixture.h"
#include "helper/parallel.h"

using klotski::array_sum;
using klotski::cases::Ranges;
using klotski::cases::BasicRanges;

using klotski::group::BLOCK_NUM;
using klotski::group::TYPE_ID_LIMIT;
using klotski::cases::BASIC_RANGES_NUM;
using klotski::cases::BASIC_RANGES_NUM_;

/// Forcibly modify private variables to reset state.
EXPOSE_VAR(BasicRanges, bool, available_)

constexpr uint64_t BASIC_RANGES_XXH3 = 0x34fce9da6a052533;

class BasicRangesTest : public testing::Test, public helper::Concurrent {
protected:
    void SetUp() override {
        Reset();
        EXPECT_FALSE(Available());
    }

    /// Whether basic ranges are ready.
    [[nodiscard]] static bool Available() {
        return BasicRanges::instance().is_available();
    }

    /// Reset basic ranges build state, note it is thread-unsafe.
    static void Reset() {
        exposer::BasicRanges_available_(BasicRanges::instance()) = false;
    }

    /// Verify that whether basic ranges data is correct.
    static void Verify() {
        const auto &basic_ranges = BasicRanges::instance().fetch();
        EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_NUM_); // verify basic ranges size
        EXPECT_EQ(helper::xxh3(basic_ranges), BASIC_RANGES_XXH3); // verify basic ranges checksum
    }
};

TEST_FF(BasicRanges, content) {
    auto &ranges = BasicRanges::instance().fetch();
    EXPECT_SORTED_AND_UNIQUE(ranges);
    EXPECT_EQ(ranges.size(), BASIC_RANGES_NUM_); // size verify
    EXPECT_EQ(helper::xxh3(ranges), BASIC_RANGES_XXH3); // checksum verify
}

TEST_FF(BasicRanges, constant) {
    EXPECT_EQ(BASIC_RANGES_NUM_, 7311885);
    EXPECT_EQ(BASIC_RANGES_NUM.size(), TYPE_ID_LIMIT);
    EXPECT_EQ(array_sum(BASIC_RANGES_NUM), BASIC_RANGES_NUM_);

    TYPE_ID_PARALLEL({
        Ranges ranges;
        const auto num = BLOCK_NUM[type_id];
        ranges.spawn(std::get<0>(num), std::get<1>(num), std::get<2>(num));
        EXPECT_EQ(ranges.size(), BASIC_RANGES_NUM[type_id]);
    });
}

TEST_FF(BasicRanges, basic_ranges) {
    BasicRanges::instance().build();
    EXPECT_TRUE(Available());
    Verify();

    BasicRanges::instance().build();
    EXPECT_TRUE(Available());
    Verify();
}

TEST_FF(BasicRanges, basic_ranges_race) {
    racer_.Execute([] {
        BasicRanges::instance().build();
    });
    EXPECT_TRUE(Available());
    Verify();
}

TEST_FF(BasicRanges, basic_ranges_async) {
    condition_.clear();
    BasicRanges::instance().build_async(executor_.Entry(), [this] {
        EXPECT_FALSE(condition_.test_and_set());
        condition_.notify_all();
    });
    EXPECT_FALSE(Available());
    condition_.wait(false);
    EXPECT_TRUE(Available());
    Verify();

    condition_.clear();
    BasicRanges::instance().build_async(executor_.Entry(), [this] {
        EXPECT_FALSE(condition_.test_and_set());
        condition_.notify_all();
    });
    EXPECT_TRUE(Available());
    condition_.wait(false);
    EXPECT_TRUE(Available());
    Verify();
}

TEST_FF(BasicRanges, basic_ranges_async_race) {
    counter_.store(0);
    racer_.Execute([this] {
        BasicRanges::instance().build_async(executor_.Entry(), [this] {
            counter_.fetch_add(1);
        });
    });
    EXPECT_TRUE(Available());
    EXPECT_EQ(counter_.load(), racer_.RaceNum());
    Verify();
}

TEST_FF(BasicRanges, basic_ranges_async_block) {
    condition_.clear();
    serial_.Entry()([this] {
        BasicRanges::instance().build_async(serial_.Entry(), [this] {
            EXPECT_FALSE(condition_.test_and_set());
            condition_.notify_all();
        });
    });
    EXPECT_FALSE(Available());
    condition_.wait(false);
    EXPECT_TRUE(Available());
    Verify();
}
