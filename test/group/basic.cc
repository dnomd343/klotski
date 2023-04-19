#include <thread>
#include <algorithm>
#include "md5.h"
#include "group.h"
#include "type_id.h"
#include "all_cases.h"
#include "tiny_pool.h"
#include "group_seeds.h"
#include "gtest/gtest.h"

#define SHOULD_PANIC(FUNC) \
    try { \
        FUNC; EXPECT_STREQ("should panic", "but no panic"); \
    } catch (...) {}

using klotski::Group;
using klotski::GroupCase;
using klotski::GroupType;

using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;

using klotski::GROUP_SEEDS;
using klotski::TYPE_ID_LIMIT;
using klotski::ALL_CASES_SIZE;
using klotski::TYPE_ID_GROUP_NUM;

const char BLOCK_NUM_MD5[] = "46a7b3af6d039cbe2f7eaebdd196c6a2";

TEST(Group, type_id) {
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        EXPECT_EQ(GroupType(type_id).unwrap(), type_id);
    }
    SHOULD_PANIC(GroupType{TYPE_ID_LIMIT}) // type id overflow

    auto test = [](uint64_t head) {
        for (auto &&range: AllCases::fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = RawCode::from_common_code(common_code);

            auto type_id = GroupType(common_code);
            EXPECT_EQ(type_id, GroupType(raw_code));
            EXPECT_LT(type_id.unwrap(), TYPE_ID_LIMIT); // type_id <= TYPE_ID_LIMIT
            EXPECT_EQ(type_id.block_num(), GroupType::block_num(raw_code));
            EXPECT_EQ(type_id.block_num(), GroupType::block_num(common_code));
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // multi-threads verify
    }
    for (auto &t : threads) { t.join(); }

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto tmp = GroupType(type_id);
        auto common_code = *tmp.cases().begin();
        EXPECT_EQ(tmp.group_num(), GroupType::group_num(common_code));
        EXPECT_EQ(tmp.group_num(), GroupType::group_num(common_code.to_raw_code()));
        EXPECT_EQ(tmp.max_size(), GroupType::max_size(common_code));
        EXPECT_EQ(tmp.max_size(), GroupType::max_size(common_code.to_raw_code()));
    }
}

TEST(Group, block_num) {
    std::string result[16];
    auto test = [&result](uint64_t head) {
        char buffer[13];
        result[head].reserve(ALL_CASES_SIZE[head]); // vector pre-allocated

        for (auto &&range: AllCases::fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto tmp = GroupType(common_code).block_num();
            EXPECT_LE(tmp.n_1x2 * 2 + tmp.n_2x1 * 2 + tmp.n_1x1, 14);
            sprintf(buffer, "%d,%d,%d\n", tmp.n_1x2 + tmp.n_2x1, tmp.n_1x1, tmp.n_2x1);
            result[head] += buffer;
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // multi-threads verify
    }
    for (auto &t : threads) { t.join(); } // build string data

    std::string block_num_str;
    for (auto &&tmp : result) {
        block_num_str += tmp; // combine result
    }
    auto block_num_md5 = md5(block_num_str.c_str(), block_num_str.size());
    EXPECT_STREQ(block_num_md5.c_str(), BLOCK_NUM_MD5); // verify md5
}

TEST(Group, group_id) {
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        for (uint32_t group_id = 0; group_id < TYPE_ID_GROUP_NUM[type_id]; ++group_id) {
            auto tmp = Group(type_id, group_id);
            EXPECT_EQ(tmp, Group(GroupType(type_id), group_id));
            EXPECT_EQ(tmp.type_id(), type_id);
            EXPECT_EQ(tmp.unwrap(), group_id);
        }
        SHOULD_PANIC(Group(type_id, TYPE_ID_GROUP_NUM[type_id])) // group_id overflow
    }
    SHOULD_PANIC(Group(TYPE_ID_LIMIT, 0)) // type_id overflow

    auto test = [](CommonCode seed) {
        auto tmp = Group(seed);
        EXPECT_EQ(tmp, Group(seed.to_raw_code()));
        EXPECT_EQ(tmp.type_id(), GroupType(seed).unwrap());

        auto size = tmp.size();
        EXPECT_EQ(size, Group::size(seed));
        EXPECT_EQ(size, Group::size(seed.to_raw_code()));

        EXPECT_EQ(seed, tmp.seed());
        EXPECT_EQ(seed, Group::seed(seed));
        EXPECT_EQ(seed, Group::seed(seed.to_raw_code()));

        auto sort = [](std::vector<RawCode> &&cases) -> std::vector<CommonCode> {
            std::vector<CommonCode> group{cases.begin(), cases.end()};
            std::sort(group.begin(), group.end());
            return group;
        };
        auto cases = sort(tmp.cases());
        EXPECT_EQ(cases, sort(Group::cases(seed)));
        EXPECT_EQ(cases, sort(Group::cases(seed.to_raw_code())));
    };

    TinyPool pool;
    for (auto &&seed : GROUP_SEEDS) {
        pool.submit(test, CommonCode(seed));
    }
    pool.boot();
    pool.join();
}

TEST(Group, operators) {
    EXPECT_EQ(GroupType(0), GroupType(0));
    EXPECT_NE(GroupType(0), GroupType(1));

    EXPECT_EQ(Group(123, 0), Group(123, 0));
    EXPECT_NE(Group(123, 0), Group(123, 1));
    EXPECT_NE(Group(123, 0), Group(124, 0));

    auto block_num = GroupType::block_num_t{0, 0, 0};
    EXPECT_EQ(block_num, (GroupType::block_num_t{0, 0, 0}));
    EXPECT_NE(block_num, (GroupType::block_num_t{1, 0, 0}));
    EXPECT_NE(block_num, (GroupType::block_num_t{0, 1, 0}));
    EXPECT_NE(block_num, (GroupType::block_num_t{0, 0, 1}));

    auto info = GroupCase::info_t{0, 0, 0};
    EXPECT_EQ(info, (GroupCase::info_t{0, 0, 0}));
    EXPECT_NE(info, (GroupCase::info_t{1, 0, 0}));
    EXPECT_NE(info, (GroupCase::info_t{0, 1, 0}));
    EXPECT_NE(info, (GroupCase::info_t{0, 0, 1}));
}
