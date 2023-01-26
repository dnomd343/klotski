/// WARN: c-style lib should not using `iostream`
///       using `printf` for screen output in test process

#include <cstdio>
#include <cstdint>

#include "klotski.h"

#include "common.h"
//#include "core.h"

void tmain() {
    printf("tmain start\n");

    uint64_t common_code = 0x1A9BC0C00;

    Common::range_reverse(common_code);

//    uint64_t raw_code = 0x0603EDF5CAFFF5E2;

//    auto core = Core([](uint64_t code, uint64_t mask) {
//        return;
//    });

//    for (uint32_t i = 0; i < 1000000; ++i) {
//        core.next_cases(raw_code, 0);
//    }

    printf("tmain exit\n");
}
