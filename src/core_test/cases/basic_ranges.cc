#include "hash.h"
#include "helper.h"

static constexpr uint64_t BASIC_RANGES_XXH3 = 0x34fce9da6a052533;

class BasicRangesTest : public testing::Test, public Concurrent {
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
        EXPECT_EQ(hash::xxh3(basic_ranges), BASIC_RANGES_XXH3); // verify basic ranges checksum
    }
};

TEST_FF(BasicRanges, constant) {
    // TODO: check BASIC_RANGES_NUM array
    EXPECT_EQ(BASIC_RANGES_NUM_, 7311885);
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
    racer_.Start([] {
        BasicRanges::instance().build();
    });
    EXPECT_FALSE(Available());
    racer_.Join();
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
    racer_.Start([this] {
        BasicRanges::instance().build_async(executor_.Entry(), [this] {
            counter_.fetch_add(1);
        });
    });
    EXPECT_FALSE(Available());
    racer_.Join();
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
