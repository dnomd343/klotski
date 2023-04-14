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

    Group::group_info(CommonCode(0x1A9BF0C00));

//    std::vector<uint32_t> group_num;
//    group_num.reserve(TYPE_ID_LIMIT);
//    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
//        group_num.emplace_back(Group::build_groups(type_id).size());
//        std::cerr << type_id << std::endl;
//    }
//
//    uint32_t offset = 0;
//    std::vector<uint32_t> group_offset;
//    group_offset.reserve(TYPE_ID_LIMIT);
//    for (auto &&tmp : group_num) {
//        group_offset.emplace_back(offset);
//        offset += tmp;
//    }
//
//    for (uint32_t i = 0; i < group_offset.size(); ++i) {
//        printf("%5d, ", group_offset[i]);
//        if (i % 12 == 11) {
//            printf("\n");
//        }
//    }

//    printf("tmain exit\n");
}
