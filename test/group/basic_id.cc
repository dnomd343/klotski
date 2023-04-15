#include <thread>
#include "md5.h"
#include "group.h"
#include "type_id.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::TypeId;
using klotski::GroupId;
using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;

using klotski::TYPE_ID_LIMIT;
using klotski::ALL_CASES_SIZE;
using klotski::TYPE_ID_GROUP_NUM;

const char BLOCK_NUM_MD5[] = "46a7b3af6d039cbe2f7eaebdd196c6a2";

TEST(Group, type_id) {
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        EXPECT_EQ(TypeId(type_id).unwrap(), type_id);
    }
    try {
        EXPECT_EQ(TypeId(TYPE_ID_LIMIT).unwrap(), -1);
    } catch (...) {} // should panic

    auto test = [](uint64_t head) {
        for (auto &&range: AllCases::fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto raw_code = RawCode::from_common_code(common_code);

            auto type_id = TypeId(common_code);
            EXPECT_EQ(type_id, TypeId(raw_code));
            EXPECT_LT(type_id.unwrap(), TYPE_ID_LIMIT);
            EXPECT_EQ(type_id.block_num(), TypeId::block_num(raw_code));
            EXPECT_EQ(type_id.block_num(), TypeId::block_num(common_code));
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // multi-threads verify
    }
    for (auto &t : threads) { t.join(); }
}

TEST(Group, block_num) {
    std::string result[16];
    auto test = [&result](uint64_t head) {
        char buffer[13];
        result[head].reserve(ALL_CASES_SIZE[head]); // vector pre-allocated
        for (auto &&range: AllCases::fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto tmp = TypeId(common_code).block_num();
            EXPECT_LE(tmp.n_1x2 * 2 + tmp.n_2x1 * 2 + tmp.n_1x1, 14);
            sprintf(buffer, "%d,%d,%d\n", tmp.n_1x2 + tmp.n_2x1, tmp.n_1x1, tmp.n_2x1);
            result[head] += buffer;
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // multi-threads verify
    }
    for (auto &t : threads) { t.join(); }

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
            auto gid = GroupId(type_id, group_id);
            EXPECT_EQ(gid, GroupId(TypeId(type_id), group_id));
            EXPECT_EQ(gid.type_id(), type_id);
            EXPECT_EQ(gid.unwrap(), group_id);
        }
        try {
            EXPECT_EQ(GroupId(type_id, TYPE_ID_GROUP_NUM[type_id]).unwrap(), -1);
        } catch (...) {} // should panic
    }

    // TODO: test GroupId(...)

}

TEST(Group, operators) {
    // TODO: TypeId
    // TODO: block_num_t
    // TODO: GroupId
}
