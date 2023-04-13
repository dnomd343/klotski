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

    std::vector<uint32_t> max_group_size;
    max_group_size.reserve(TYPE_ID_LIMIT);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
//        std::cout << Group::build_groups(type_id)[0].size() << std::endl;
        max_group_size.emplace_back(Group::build_groups(type_id)[0].size());
        std::cerr << type_id << std::endl;
    }

    for (uint32_t i = 0; i < max_group_size.size(); ++i) {

        printf("%6d, ", max_group_size[i]);
        if (i % 12 == 11) {
            printf("\n");
        }

    }

//    printf("tmain exit\n");
}
