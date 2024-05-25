#include <hash.h>
#include <group/group.h>
#include <gtest/gtest.h>
#include <ranges/ranges.h>
#include <__format/format_functions.h>

TEST(Group, group_extend) {
    // auto src = klotski::codec::RawCode::from_common_code(0x1A9BF0C00)->unwrap();
    // auto codes = klotski::cases::group_extend_from_seed(src);
    //
    // for (auto &code: codes) {
    //     auto raw_code = klotski::codec::RawCode::create(code).value();
    //     code = raw_code.to_common_code().unwrap();
    // }
    // std::sort(codes.begin(), codes.end());
    // EXPECT_EQ(codes.size(), 25955);
    //
    // auto index = std::lower_bound(codes.begin(), codes.end(), 0x1A9BF0C00) - codes.begin();
    // EXPECT_EQ(index, 7472);
    //
    // auto hash_ret = hash::xxh3(codes.data(), codes.size() * sizeof(uint64_t));
    // EXPECT_EQ(hash_ret, 0x91BD28A749312A6D);
}

static std::vector<std::tuple<int, int, int>> target_nums() {

    std::vector<std::tuple<int, int, int>> results;

    for (int n = 0; n <= 7; ++n) {
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) {
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) {
                results.emplace_back(n, n_2x1, n_1x1);
            }
        }
    }

    results.resize(203);
    return results;
}

TEST(Group, ranges) {
    // auto ret = klotski::cases::spawn_ranges(2, 1, 4, 4);
    //
    // EXPECT_EQ(ret.size(), 34650);
    //
    // auto hash_ret = hash::xxh3(ret.data(), ret.size() * 4);
    // EXPECT_EQ(hash_ret, 0xF6F87606E4205EAF);

    klotski::cases::Ranges kk {};

    // std::vector<uint32_t> ranges;
    for (auto [n, n_2x1, n_1x1] : target_nums()) {

        kk.spawn(n, n_2x1, n_1x1);

        // kk.spawn_more(n, n_2x1, n_1x1);

        // auto kk = klotski::cases::spawn_ranges(n, n_2x1, n_1x1);

        // ranges.insert(ranges.end(), kk.begin(), kk.end());
    }

    EXPECT_EQ(kk.size(), 7311885);

    klotski::cases::Ranges pp {kk};
    for (auto &x : pp) {
        x = klotski::range_reverse(x);
    }

    // auto hash_ret = hash::xxh3(kk.ranges_);
    // EXPECT_EQ(hash_ret, 0xA1E247B01D5A9545);

    klotski::cases::Ranges result;
    // kk.derive(5, result);
    klotski::cases::derive_demo(kk, pp, result, 5);

    EXPECT_EQ(result.size(), 1355971);

    // std::cout << result.size() << std::endl;

    EXPECT_EQ(hash::xxh3(result), 0xf00ecc84b6f678a6);

    // std::cout << std::format("{:x}", hash::xxh3(result)) << std::endl;


}

TEST(Group, basic_ranges) {
    // auto ret = klotski::cases::basic_ranges();
    // std::cout << ret.size() << std::endl;

    // EXPECT_EQ(ret.size(), 7311921);

    // auto hash_ret = hash::xxh3(ret);
    // std::cout << std::format("{:X}", hash_ret) << std::endl;

    // EXPECT_EQ(hash_ret, 0xA1E247B01D5A9545); // no sorted
    // EXPECT_EQ(hash_ret, 0x00A926AB1121230D); // no reversed
    // EXPECT_EQ(hash_ret, 0x82B040060044E336);
}
