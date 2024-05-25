#include <hash.h>
#include <group/group.h>
#include <gtest/gtest.h>
#include <__format/format_functions.h>

TEST(Group, group_extend) {
    auto src = klotski::codec::RawCode::from_common_code(0x1A9BF0C00)->unwrap();
    auto codes = klotski::cases::group_extend_from_seed(src);

    for (auto &code: codes) {
        auto raw_code = klotski::codec::RawCode::create(code).value();
        code = raw_code.to_common_code().unwrap();
    }
    std::sort(codes.begin(), codes.end());
    EXPECT_EQ(codes.size(), 25955);

    auto index = std::lower_bound(codes.begin(), codes.end(), 0x1A9BF0C00) - codes.begin();
    EXPECT_EQ(index, 7472);

    auto hash_ret = hash::xxh3(codes.data(), codes.size() * sizeof(uint64_t));
    EXPECT_EQ(hash_ret, 0x91BD28A749312A6D);
}
