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
using klotski::GroupCase;
using klotski::GroupType;

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::Benchmark;
using klotski::BasicRanges;

using klotski::TYPE_ID_LIMIT;

void tmain() {
//    printf("tmain start\n");

//    auto start = clock();

    auto demo = []() {
        // 1A9BF0C00 -> 169-1-7472
        // 4FEA13400 -> 164-0-30833
        std::cout << GroupCase::parse({169, 1, 7472}) << std::endl;
        std::cout << GroupCase::parse({164, 0, 30833}) << std::endl;
        auto i1 = GroupCase::encode(CommonCode(0x1A9BF0C00));
        auto i2 = GroupCase::encode(CommonCode(0x4FEA13400));
        std::cout << i1.type_id << "-" << i1.group_id << "-" << i1.group_index << std::endl;
        std::cout << i2.type_id << "-" << i2.group_id << "-" << i2.group_index << std::endl;
    };

    demo();
    GroupCase::speed_up();
    demo();

//    std::cerr << (clock() - start) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;

//    printf("tmain exit\n");
}
