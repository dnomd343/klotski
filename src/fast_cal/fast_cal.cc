#include "core.h"
#include "common.h"
#include "fast_cal.h"

#include "raw_code.h"

#include <iostream>

void FastCal::new_case(uint64_t code, uint64_t mask) { // callback function for new case
    auto current = cases.find(code);
    if (current != cases.end()) { // find existed case
        current->second.mask |= mask; // update mask info
        return;
    }
    cases[code] = fast_cal_t { // insert into cases map
        .code = code,
        .mask = mask,
        .last = cache.front(), // parent case
    };
    cache.emplace(&cases[code]); // add in working queue
}

void FastCal::fast_cal(uint64_t code) {

    auto core = Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
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
