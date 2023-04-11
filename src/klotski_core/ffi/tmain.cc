#include <cstdio>
#include <iostream>
#include <algorithm>

#include <set>
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


void tmain() {
//    printf("tmain start\n");

//    auto c = CommonCode(0x1A9BF0C00);

//    auto groups = Group::build_groups(Group::type_id(c));
//    for (uint32_t i = 0; i < groups.size(); ++i) {
//        std::cout << g.size() << std::endl;
//        if (std::find(groups[i].begin(), groups[i].end(), c) != groups[i].end()) {
//            std::cout << i << " -> " << groups[i].size() << std::endl;
//        } else {
//            std::cout << "not found" << std::endl;
//        }
//
//    }

    struct group_info_t {
        uint16_t type_id;
        uint16_t group_id;
        uint32_t group_index;
    };

//    std::unordered_map<CommonCode, group_info_t> data;

    std::vector<group_info_t> data;

    data.resize(klotski::SHORT_CODE_LIMIT);

    ShortCode::speed_up(ShortCode::FAST);

//    uint16_t type_id = 123;

    for (uint16_t type_id = 0; type_id < klotski::TYPE_ID_LIMIT; ++type_id) {
        auto groups = Group::build_groups(type_id);
        for (uint32_t id = 0; id < groups.size(); ++id) {

            std::sort(groups[id].begin(), groups[id].end());

            for (uint32_t index = 0; index < groups[id].size(); ++index) {
                data[groups[id][index].to_short_code().unwrap()] = {
                    .type_id = type_id,
                    .group_id = static_cast<uint16_t>(id),
                    .group_index = index,
                };
            }
        }
//        std::cout << type_id << std::endl;
        std::cerr << type_id << std::endl;
    }

//    std::cout << data.size() << std::endl;

    for (auto &&tmp : data) {
        std::cout << tmp.group_index << std::endl;
    }


//    for (auto &&g : Group::build_groups(123)) {

//        std::cout << *std::min_element(g.begin(), g.end()) << std::endl;

//        std::cout << g.size() << std::endl;
//    }

//    for (uint32_t type_id = 0; type_id < 204; ++type_id) {
//        std::cout << Group::build_groups(type_id).size() << std::endl;
//    }

//    printf("tmain exit\n");
}
