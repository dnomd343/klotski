#include <gtest/gtest.h>

#include <format>
#include <cstdint>
#include <algorithm>

// TODO: only for debug
#include <iostream>

#include "group/group.h"
#include "helper/cases.h"
#include "common_code/common_code.h"

TEST(GroupUnion, demo) {

    // for (auto i = 0; i < block_nums().size(); ++i) {
    //     std::cout << block_nums()[i].n_2x1 + block_nums()[i].n_1x2 << ", ";
    //     std::cout << block_nums()[i].n_2x1 << ", ";
    //     std::cout << block_nums()[i].n_1x1 << std::endl;
    // }

    std::vector<std::vector<uint64_t>> pp;

    pp.resize(block_nums().size());

    for (uint64_t head = 0; head < 16; ++head) {

        for (auto range : AllCases::instance().fetch()[head]) {
            uint64_t common_code = head << 32 | range;

            auto type_id = to_type_id(cal_block_num(common_code));

            pp[type_id].emplace_back(common_code);

        }

    }

    for (uint32_t type_id = 0; type_id < klotski::cases::TYPE_ID_LIMIT; ++type_id) {

        auto cases = klotski::cases::GroupUnion::unsafe_create(type_id).cases();

        std::vector<uint64_t> extend {};
        for (uint64_t head = 0; head < 16; ++head) {
            for (auto range : cases[head]) {
                extend.emplace_back(head << 32 | range);
            }
        }

        std::cout << "type_id " << type_id << " -> " << extend.size() << std::endl;

        EXPECT_EQ(extend, pp[type_id]);
    }


}
