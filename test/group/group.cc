#include <thread>
#include <algorithm>
#include "md5.h"
#include "group.h"
#include "gtest/gtest.h"

#include "group/size.h"

using klotski::Group;
using klotski::AllCases;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::TYPE_ID_LIMIT;
using klotski::ALL_CASES_SIZE_SUM;
using klotski::GROUP_ALL_CASES_SIZE;

using klotski::TYPE_ID_LIMIT;
using klotski::SHORT_CODE_LIMIT;


const char BLOCK_NUM_MD5[] = "46a7b3af6d039cbe2f7eaebdd196c6a2";

const char GROUP_INFO_MD5[] = "976bf22530085210e68a6a4e67053506";

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

TEST(Group, all_cases) {
    std::vector<std::vector<CommonCode>> all_cases;
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        all_cases.emplace_back(Group::all_cases(type_id)); // build test data
    }

    std::vector<uint64_t> combine_cases;
    combine_cases.reserve(ALL_CASES_SIZE_SUM);
    for (uint32_t id = 0; id < TYPE_ID_LIMIT; ++id) {
        EXPECT_EQ(all_cases[id].size(), GROUP_ALL_CASES_SIZE[id]); // verify cases number
        for (auto &&common_code : all_cases[id]) {
            EXPECT_EQ(Group::type_id(common_code), id); // verify type id
            combine_cases.emplace_back(common_code.unwrap());
        }
        std::is_sorted(all_cases[id].begin(), all_cases[id].end()); // verify data order
    }
    EXPECT_EQ(combine_cases.size(), ALL_CASES_SIZE_SUM); // verify sum

    auto all_cases_release = AllCases::release();
    std::stable_sort(combine_cases.begin(), combine_cases.end());
    for (uint32_t i = 0; i < combine_cases.size(); ++i) {
        EXPECT_EQ(combine_cases[i], all_cases_release[i]); // verify after combined
    }
}

TEST(Group, build_groups) {
    struct group_info_t {
        uint16_t group_id;
        uint32_t group_index;
    };
    ShortCode::speed_up(ShortCode::FAST);
    std::vector<group_info_t> all_cases(SHORT_CODE_LIMIT);

    std::vector<uint16_t> group_ids;

    for (uint16_t type_id = 0; type_id < klotski::TYPE_ID_LIMIT; ++type_id) {
        std::vector<uint32_t> group_sizes;
        std::vector<CommonCode> group_seeds;
        auto groups = Group::build_groups(type_id);
        group_sizes.reserve(groups.size());
        group_seeds.reserve(groups.size());

        for (uint32_t id = 0; id < groups.size(); ++id) {
            group_sizes.emplace_back(groups[id].size()); // record size
            std::sort(groups[id].begin(), groups[id].end()); // sort for group index
            for (uint32_t index = 0; index < groups[id].size(); ++index) {
                all_cases[groups[id][index].to_short_code().unwrap()] = {
                    .group_id = static_cast<uint16_t>(id),
                    .group_index = index,
                };
                EXPECT_EQ(Group::type_id(groups[id][index]), type_id); // verify type id
            }
            group_seeds.emplace_back(groups[id][0]); // record seed
        }

        // TODO: check group_seeds and group_sizes

        // TODO: verify GROUP_OFFSET

    }

    // TODO: verify GROUP_SEEDS / GROUP_SEEDS_INDEX / GROUP_SEEDS_INDEX_REV

    char buffer[8];
    std::string group_info;
    for (auto &&tmp : all_cases) {
        sprintf(buffer, "%d,%d\n", tmp.group_id, tmp.group_index);
        group_info += buffer;
    }
    auto group_info_md5 = md5(group_info.c_str(), group_info.size());
    EXPECT_STREQ(group_info_md5.c_str(), GROUP_INFO_MD5); // verify all group info
}
