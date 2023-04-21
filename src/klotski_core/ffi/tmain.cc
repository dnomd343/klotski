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

    GroupCase::demo();

//    auto ret = GroupCase::encode(CommonCode(0x1A9BF0C00));
//    auto ret = GroupCase::encode(CommonCode(0x4FEA13400));
//
//    std::cout << "type id: " << ret.type_id << std::endl;
//    std::cout << "group id: " << ret.group_id << std::endl;
//    std::cout << "group index: " << ret.group_index << std::endl;

//    std::cout << GroupCase::parse({
//        .type_id = 169,
//        .group_id = 1,
//        .group_index = 7472,
//    }) << std::endl;
//
//    std::cout << GroupCase::parse({
//         .type_id = 164,
//         .group_id = 0,
//         .group_index = 30833,
//    }) << std::endl;

//    std::cerr << (clock() - start) * 1000 / CLOCKS_PER_SEC << "ms" << std::endl;

//    printf("tmain exit\n");
}
