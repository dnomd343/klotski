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

    std::vector<uint32_t> group_num;
    group_num.reserve(TYPE_ID_LIMIT);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        group_num.emplace_back(Group::build_groups(type_id).size());
        std::cerr << type_id << std::endl;
    }

    for (uint32_t i = 0; i < group_num.size(); ++i) {

        printf("%4d, ", group_num[i]);
        if (i % 16 == 15) {
            printf("\n");
        }

    }

//    printf("tmain exit\n");
}
