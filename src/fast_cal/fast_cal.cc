#include "core.h"
#include "common.h"
#include "fast_cal.h"

#include "raw_code.h"

#include <iostream>

void FastCal::fast_cal(uint64_t code) {

//    std::cout << RawCode(code).dump_case() << std::endl;

    auto core = Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            add_new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );

    cases.empty();
    cache.empty();

    cases[code] = fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr,
    };
    cache.emplace(&cases[code]);

    while (!cache.empty()) {

        /// break check point

        if (((cache.front()->code >> (3 * 0xD)) & 0b111) == B_2x2) {
            std::cout << "Resolved" << std::endl;
            break;
        }

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    auto solution = cache.front();

    while (solution != nullptr) {
//        std::cout << RawCode(solution->code).dump_case() << std::endl;
        solution = solution->last;
    }

}

void FastCal::add_new_case(uint64_t code, uint64_t mask) {

    auto exist_case = cases.find(code);
    if (exist_case != cases.end()) { // find existed case

        exist_case->second.mask |= mask; // mask update
        return;

    }

    cases[code] = fast_cal_t {
        .code = code,
        .mask = mask,
        .last = cache.front(),
    };
    cache.emplace(&cases[code]);

}
