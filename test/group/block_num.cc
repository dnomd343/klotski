#include <thread>
#include "md5.h"
#include "group.h"
#include "all_cases.h"
#include "gtest/gtest.h"

using klotski::Group;
using klotski::AllCases;
using klotski::CommonCode;
using klotski::TYPE_ID_LIMIT;

const char BLOCK_NUM_MD5[] = "46a7b3af6d039cbe2f7eaebdd196c6a2";

TEST(Group, type_id) {
    std::thread threads[16];
    auto test = [](uint64_t head) {
        for (const auto &range : AllCases::fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto type_id = Group::type_id(common_code); // found type id
            EXPECT_LT(type_id, TYPE_ID_LIMIT);
            EXPECT_EQ(type_id, Group::type_id(common_code.to_raw_code()));
            EXPECT_EQ(type_id, Group::type_id(Group::block_num(common_code)));
            EXPECT_EQ(Group::block_num(type_id), Group::block_num(common_code));
        }
    };
    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
}

TEST(Group, block_num) {
    std::thread threads[16];
    std::string block_num_data;
    std::string block_num_str[16];

    auto test = [&block_num_str](uint64_t head) {
        char buffer[13];
        for (auto &&range: AllCases::fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);
            auto tmp = Group::block_num(common_code);
            EXPECT_LE(tmp.n_1x2 * 2 + tmp.n_2x1 * 2 + tmp.n_1x1, 14);
            EXPECT_EQ(tmp, Group::block_num(common_code.to_raw_code()));
            sprintf(buffer, "%d,%d,%d\n", tmp.n_1x2 + tmp.n_2x1, tmp.n_1x1, tmp.n_2x1);
            block_num_str[head] += buffer;
        }
    };

    for (uint64_t head = 0; head < 16; ++head) { // split into 16 threads
        threads[head] = std::thread(test, head);
    }
    for (auto &t : threads) {
        t.join();
    }
    for (auto &&tmp : block_num_str) { // combine string
        block_num_data += tmp;
    }
    auto block_num_md5 = md5(block_num_data.c_str(), block_num_data.size());
    EXPECT_STREQ(block_num_md5.c_str(), BLOCK_NUM_MD5);
}
