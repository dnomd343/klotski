#include <cstdio>
#include <iostream>
#include <algorithm>

#include <set>
#include <unordered_set>

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
using klotski::CommonCode;

using klotski::Benchmark;
using klotski::BasicRanges;

void tmain() {
//    printf("tmain start\n");

//    auto ret = Group::build_groups(0);
//    std::cout << ret[0].size() << std::endl;

//    auto g = Group::build_groups(123);
//    std::cout << g.size() << std::endl;

    for (uint32_t type_id = 0; type_id < 204; ++type_id) {
        std::cout << Group::build_groups(type_id).size() << std::endl;
    }

//    printf("tmain exit\n");
}
