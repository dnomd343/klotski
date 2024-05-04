#include "hash.h"
#include "helper.h"

static constexpr uint64_t BASIC_RANGES_XXH3 = 0x82b040060044e336;

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
        EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_NUM); // verify basic ranges size
        EXPECT_EQ(hash::xxh3(basic_ranges), BASIC_RANGES_XXH3); // verify basic ranges checksum
    }
};

TEST_FF(BasicRanges, constant) {
    EXPECT_EQ(BASIC_RANGES_NUM, 7311921);
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
    racer_.Begin([] {
        BasicRanges::instance().build();
    });
    EXPECT_FALSE(Available());
    racer_.Join();
    EXPECT_TRUE(Available());
    Verify();
}