#include <iostream>
#include <unordered_map>
#include <queue>
#include "core.h"
#include "fast_cal.h"
#include "raw_code.h"
#include "common.h"

struct fast_cal_t {
    uint64_t code;
    uint64_t mask;
//    uint32_t step;
};

std::queue<fast_cal_t*> cache;

std::unordered_map<uint64_t, fast_cal_t> cases;

void add_new_case(uint64_t code, uint64_t mask) {

    auto exist_case = cases.find(code);
    if (exist_case != cases.end()) { // find existed case

        exist_case->second.mask |= mask; // mask update
        return;

    }

    cases[code] = fast_cal_t {
        .code = code,
        .mask = mask,
    };;
    cache.emplace(&cases[code]);
}

uint32_t fast_cal(uint64_t code) {

    auto core = Core(add_new_case);

    cases.empty();

    cases[code] = fast_cal_t {
        .code = code,
        .mask = 0,
    };
    cache.emplace(&cases[code]);

    while (!cache.empty()) {

        if (((cache.front()->code >> (3 * 0xD)) & 0b111) == B_2x2) {
            std::cout << "Resolve" << std::endl;
            std::cout << RawCode(cache.front()->code).dump_case() << std::endl;
            break;
        }

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    return cases.size();

}
