#include <vector>

#include "hash.h"
#include "exposer.h"
#include "all_cases.h"
#include "concurrent.h"
#include "gtest/gtest.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::cases::ALL_CASES_NUM;
using klotski::cases::ALL_CASES_NUM_;
using klotski::cases::BASIC_RANGES_NUM;

static constexpr uint64_t BASIC_RANGES_XXH3 = 0x82b040060044e336;

static constexpr std::array<uint64_t, 16> ALL_CASES_XXH3 = {
    0x71c8ff7a71c93da0, 0x2a5247ee8bfed666, 0xf4efc8fc692d58e2, 0x2d06800538d394c2,
    0xb3f7cc1b962d6944, 0x7e2792f8ab777faa, 0x4b8e78026cca8a27, 0x2d06800538d394c2,
    0x8acd688c5ab93c42, 0xedca5101ed81cc77, 0xe8dc9d30c91ce682, 0x2d06800538d394c2,
    0x2cdf6c14a7ce3e9a, 0xb9dd04a315583f5c, 0x19046e49c44ae90d, 0x2d06800538d394c2,
};

/// Forcibly modify private variables to reset state.
PRIVATE_ACCESS(AllCases, available_, bool)
PRIVATE_ACCESS(BasicRanges, available_, bool)

class BR_Test final {
public:
    [[nodiscard]] static bool available() {
        return BasicRanges::instance().is_available();
    }

    // TODO: expect member function

    /// Reset basic ranges build state, note it is thread-unsafe.
    static void reset() {
        access_BasicRanges_available_(BasicRanges::instance()) = false;
    }

    /// Verify that whether basic ranges data is correct.
    static void verify() {
        const auto &basic_ranges = BasicRanges::instance().fetch();
        EXPECT_EQ(basic_ranges.size(), BASIC_RANGES_NUM); // verify basic ranges size
        EXPECT_EQ(hash::xxh3(basic_ranges), BASIC_RANGES_XXH3); // verify basic ranges checksum
    }
};

class AC_Test final {
public:
    [[nodiscard]] static bool available() {
        return AllCases::instance().is_available();
    }

    /// Reset all cases build state, note it is thread-unsafe.
    static void reset() {
        access_AllCases_available_(AllCases::instance()) = false;
    }

    /// Verify that whether all cases data is correct.
    static void verify() {
        const auto &all_cases = AllCases::instance().fetch();
        for (int head = 0; head < 16; ++head) {
            EXPECT_EQ(all_cases[head].size(), ALL_CASES_NUM[head]); // verify all cases size
        }

        auto all_cases_num = 0;
        std::for_each(all_cases.begin(), all_cases.end(), [&all_cases_num](auto &ranges) {
            all_cases_num += ranges.size();
        });
        EXPECT_EQ(all_cases_num, ALL_CASES_NUM_); // verify all cases global size

        for (uint64_t head = 0; head < 16; ++head) {
            EXPECT_EQ(hash::xxh3(all_cases[head]), ALL_CASES_XXH3[head]); // verify all cases checksum
        }
    }
};

TEST(AllCases, basic_ranges) {
    BR_Test::reset();
    EXPECT_FALSE(BR_Test::available());

    BasicRanges::instance().build();
    EXPECT_TRUE(BR_Test::available());

    BasicRanges::instance().build();
    EXPECT_TRUE(BR_Test::available());
    BR_Test::verify();
}

TEST(AllCases, basic_ranges_race) {
    BR_Test::reset();
    EXPECT_FALSE(BR_Test::available());

    auto racer = co::Racer([] {
        BasicRanges::instance().build();
    });
    EXPECT_FALSE(BR_Test::available());

    racer.Join();
    EXPECT_TRUE(BR_Test::available());
    BR_Test::verify();
}

TEST(AllCases, all_cases) {
    AC_Test::reset();
    EXPECT_FALSE(AC_Test::available());

    AllCases::instance().build();
    EXPECT_TRUE(AC_Test::available());

    AllCases::instance().build();
    EXPECT_TRUE(AC_Test::available());
    AC_Test::verify();
}

TEST(AllCases, all_cases_race) {
    AC_Test::reset();
    EXPECT_FALSE(AC_Test::available());

    auto racer = co::Racer([] {
        AllCases::instance().build();
    });
    EXPECT_FALSE(AC_Test::available());

    racer.Join();
    EXPECT_TRUE(AC_Test::available());
    AC_Test::verify();
}

TEST(AllCases, all_cases_parallel) {
    AC_Test::reset();
    EXPECT_FALSE(AC_Test::available());

    co::Executor executor;
    AllCases::instance().build_parallel(executor.Entry());
    EXPECT_TRUE(AC_Test::available());

    AllCases::instance().build_parallel(executor.Entry());
    EXPECT_TRUE(AC_Test::available());
    AC_Test::verify();
}

TEST(AllCases, all_cases_parallel_race) {
    AC_Test::reset();
    EXPECT_FALSE(AC_Test::available());

    co::Executor executor;
    auto racer = co::Racer([&executor] {
        AllCases::instance().build_parallel(executor.Entry());
    });
    EXPECT_FALSE(AC_Test::available());

    racer.Join();
    EXPECT_TRUE(AC_Test::available());
    AC_Test::verify();
}

TEST(AllCases, all_cases_async) {
    AC_Test::reset();
    EXPECT_FALSE(AC_Test::available());

    co::Executor executor;
    std::atomic_flag flag;

    flag.clear();
    AllCases::instance().build_parallel_async(executor.Entry(), [&flag]() {
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_FALSE(AC_Test::available());

    flag.wait(false);
    EXPECT_TRUE(AC_Test::available());

    flag.clear();
    AllCases::instance().build_parallel_async(executor.Entry(), [&flag]() {
        flag.test_and_set();
        flag.notify_all();
    });
    EXPECT_TRUE(AC_Test::available());

    flag.wait(false);
    EXPECT_TRUE(AC_Test::available());
    AC_Test::verify();
}

TEST(AllCases, all_cases_async_race) {
    AC_Test::reset();
    EXPECT_FALSE(AC_Test::available());

    co::Executor executor;
    std::atomic<int> callback_num(0);

    auto racer = co::Racer([&executor, &callback_num] {
        AllCases::instance().build_parallel_async(executor.Entry(), [&callback_num]() {
            callback_num.fetch_add(1);
        });
    });
    EXPECT_FALSE(AC_Test::available());

    racer.Join();
    EXPECT_TRUE(AC_Test::available());
    EXPECT_EQ(callback_num.load(), co::Racer::Times);
    AC_Test::verify();
}
