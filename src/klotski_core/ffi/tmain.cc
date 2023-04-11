#include <cstdio>
#include <iostream>
#include <algorithm>

#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "klotski.h"
#include "core.h"

#include "common.h"
#include "benchmark.h"

#include "all_cases.h"
#include "common_code.h"

#include "group.h"

using klotski::Group;
using klotski::AllCases;

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::Benchmark;
using klotski::BasicRanges;

using klotski::TYPE_ID_LIMIT;

void tmain() {
//    printf("tmain start\n");

//    struct group_info_t {
//        uint16_t type_id;
//        uint16_t group_id;
//        uint32_t group_index;
//    };
//
//    std::vector<group_info_t> data;
//    data.resize(klotski::SHORT_CODE_LIMIT);
//    ShortCode::speed_up(ShortCode::FAST);
//
//    for (uint16_t type_id = 0; type_id < klotski::TYPE_ID_LIMIT; ++type_id) {
//        auto groups = Group::build_groups(type_id);
//        for (uint32_t id = 0; id < groups.size(); ++id) {
//
//            std::sort(groups[id].begin(), groups[id].end());
//
//            for (uint32_t index = 0; index < groups[id].size(); ++index) {
//                data[groups[id][index].to_short_code().unwrap()] = {
//                    .type_id = type_id,
//                    .group_id = static_cast<uint16_t>(id),
//                    .group_index = index,
//                };
//            }
//        }
//        std::cerr << type_id << std::endl;
//    }
//
//    for (auto &&tmp : data) {
//        std::cout << tmp.group_index << std::endl;
//    }

//    std::vector<uint32_t> group_num;
//    for (uint16_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
//        group_num.emplace_back(Group::build_groups(type_id).size());
//    }
//    for (auto &&tmp : group_num) {
//        std::cout << tmp << std::endl;
//    }

    for (uint16_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto groups = Group::build_groups(type_id);
        std::map<CommonCode, uint32_t> seeds;
        for (uint32_t group_id = 0; group_id < groups.size(); ++group_id) {
            auto seed = *std::min_element(groups[group_id].begin(), groups[group_id].end());
            seeds.emplace(std::pair<CommonCode, uint32_t>(seed, group_id));
        }

//        std::cout << seeds.size() << std::endl;

        for (auto &&tmp: seeds) {
//        std::cout << tmp.first << " -> " << tmp.second << std::endl;
            std::cout << tmp.first << std::endl;
        }

        std::cerr << type_id << std::endl;

    }


//    printf("tmain exit\n");
}
