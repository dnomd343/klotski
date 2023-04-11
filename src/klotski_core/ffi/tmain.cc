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

    Group::build_groups(123);

//    printf("tmain exit\n");
}
