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
    fast_cal_t *last;
};

std::queue<fast_cal_t*> cache;

std::unordered_map<uint64_t, fast_cal_t> cases;

//bool stop_flag;

void add_new_case(uint64_t code, uint64_t mask) {

//    if (stop_flag) {
//        return;
//    }

    auto exist_case = cases.find(code);
    if (exist_case != cases.end()) { // find existed case

        exist_case->second.mask |= mask; // mask update
        return;

    }

    cases[code] = fast_cal_t {
        .code = code,
        .mask = mask,
//        .step = cache.front()->step + 1,
        .last = cache.front(),
    };;
    cache.emplace(&cases[code]);

    // TODO: try to check head address = 0xD at here

//    if (((code >> (3 * 0xD)) & 0b111) == B_2x2) {
//        std::cout << "Resolved" << std::endl;
//            std::cout << RawCode(cache.front()->code).dump_case() << std::endl;
//        stop_flag = true;
//    }

}

uint32_t fast_cal(uint64_t code) {

    auto core = Core(add_new_case);

    cases.empty();

    cache.empty();

//    stop_flag = false;

    cases[code] = fast_cal_t {
        .code = code,
        .mask = 0,
//        .step = 0,
        .last = nullptr,
    };
    cache.emplace(&cases[code]);

    while (!cache.empty()) {

        // break check point
        if (((cache.front()->code >> (3 * 0xD)) & 0b111) == B_2x2) {
            std::cout << "Resolved" << std::endl;
//            std::cout << RawCode(cache.front()->code).dump_case() << std::endl;
            break;
        }

//        if (stop_flag) {
//            break;
//        }

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    auto solution = cache.front();

    while (solution != nullptr) {
        std::cout << RawCode(solution->code).dump_case() << std::endl;
        solution = solution->last;
    }

    return cases.size();

}
