#include <gtest/gtest.h>

#include "helper/hash.h"
#include "helper/cases.h"
#include "utility/exposer.h"

#include "short_code/short_code.h"
#include "common_code/common_code.h"

using klotski::array_sum;
using klotski::cases::Ranges;
using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::cases::ALL_CASES_NUM_;
using klotski::codec::SHORT_CODE_LIMIT;

/// Forcibly modify private variables to reset state.
EXPOSE_VAR(AllCases, bool, available_)

constexpr auto ALL_CASES_XXH3 = std::to_array<uint64_t>({
    0x71c8ff7a71c93da0, 0x2a5247ee8bfed666, 0xf4efc8fc692d58e2, 0x2d06800538d394c2,
    0xb3f7cc1b962d6944, 0x7e2792f8ab777faa, 0x4b8e78026cca8a27, 0x2d06800538d394c2,
    0x8acd688c5ab93c42, 0xedca5101ed81cc77, 0xe8dc9d30c91ce682, 0x2d06800538d394c2,
    0x2cdf6c14a7ce3e9a, 0xb9dd04a315583f5c, 0x19046e49c44ae90d, 0x2d06800538d394c2,
});

class AllCasesTest : public testing::Test, public Concurrent {
protected:
    void SetUp() override {
        Reset();
        EXPECT_FALSE(Available());
    }

    /// Whether all cases are ready.
    [[nodiscard]] static bool Available() {
        return AllCases::instance().is_available();
    }

    /// Reset all cases build state, note it is thread-unsafe.
    static void Reset() {
        exposer::AllCases_available_(AllCases::instance()) = false;
    }

    /// Verify that whether all cases data is correct.
    static void Verify() {
        const auto &all_cases = AllCases::instance().fetch();
        for (int head = 0; head < 16; ++head) {
            EXPECT_EQ(all_cases[head].size(), ALL_CASES_NUM[head]); // verify all cases size
            EXPECT_EQ(helper::xxh3(all_cases[head]), ALL_CASES_XXH3[head]); // verify all cases checksum
        }
    }
};

TEST_FF(AllCases, content) {
    for (auto head : Heads) {
        auto &cases = AllCases::instance().fetch()[head];
        EXPECT_SORTED_AND_UNIQUE(cases);
        EXPECT_EQ(cases.size(), ALL_CASES_NUM[head]); // size verify
        EXPECT_EQ(helper::xxh3(cases), ALL_CASES_XXH3[head]); // checksum verify
        EXPECT_SUBSET(BasicRanges::instance().fetch(), cases); // subset verify
        EXPECT_COMMON_CODES(head, cases); // release verify
    }

    ShortCode::speed_up(true);
    std::vector<uint32_t> short_codes;
    short_codes.reserve(ALL_CASES_NUM_);
    for (const auto code : AllCases::instance().fetch().codes()) {
        short_codes.emplace_back(code.to_short_code().unwrap());
    }
    EXPECT_EQ(short_codes.front(), 0);
    EXPECT_SORTED_AND_UNIQUE(short_codes);
    EXPECT_EQ(short_codes.back(), SHORT_CODE_LIMIT - 1);
}

TEST_FF(AllCases, constant) {
    EXPECT_EQ(ALL_CASES_NUM_, 29334498);
    EXPECT_EQ(ALL_CASES_NUM.size(), 16);
    EXPECT_EQ(array_sum(ALL_CASES_NUM), ALL_CASES_NUM_);

    EXPECT_EQ(ALL_CASES_NUM[3], 0);
    EXPECT_EQ(ALL_CASES_NUM[7], 0);
    EXPECT_EQ(ALL_CASES_NUM[11], 0);
    EXPECT_EQ(ALL_CASES_NUM[15], 0);

    auto ranges = BasicRanges::instance().fetch();
    ranges.reverse();
    for (const auto head : Heads) {
        Ranges release;
        ranges.derive(head, release);
        EXPECT_EQ(release.size(), ALL_CASES_NUM[head]);
    }
}

TEST_FF(AllCases, all_cases) {
    AllCases::instance().build();
    EXPECT_TRUE(Available());
    Verify();

    AllCases::instance().build();
    EXPECT_TRUE(Available());
    Verify();
}

TEST_FF(AllCases, all_cases_race) {
    racer_.Start([] {
        AllCases::instance().build();
    });
    EXPECT_FALSE(Available());
    racer_.Join();
    EXPECT_TRUE(Available());
    Verify();
}

TEST_FF(AllCases, all_cases_async) {
    condition_.clear();
    AllCases::instance().build_async(executor_.Entry(), [this] {
        EXPECT_FALSE(condition_.test_and_set());
        condition_.notify_all();
    });
    EXPECT_FALSE(Available());
    condition_.wait(false);
    EXPECT_TRUE(Available());
    Verify();

    condition_.clear();
    AllCases::instance().build_async(executor_.Entry(), [this] {
        EXPECT_FALSE(condition_.test_and_set());
        condition_.notify_all();
    });
    EXPECT_TRUE(Available());
    condition_.wait(false);
    EXPECT_TRUE(Available());
    Verify();
}

TEST_FF(AllCases, all_cases_async_race) {
    counter_.store(0);
    racer_.Start([this] {
        AllCases::instance().build_async(executor_.Entry(), [this] {
            counter_.fetch_add(1);
        });
    });
    EXPECT_FALSE(Available());
    racer_.Join();
    EXPECT_TRUE(Available());
    EXPECT_EQ(counter_.load(), racer_.RaceNum());
    Verify();
}

TEST_FF(AllCases, all_cases_async_block) {
    condition_.clear();
    serial_.Entry()([this] {
        AllCases::instance().build_async(serial_.Entry(), [this] {
            EXPECT_FALSE(condition_.test_and_set());
            condition_.notify_all();
        });
    });
    EXPECT_FALSE(Available());
    condition_.wait(false);
    EXPECT_TRUE(Available());
    Verify();
}
