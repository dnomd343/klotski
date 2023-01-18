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

    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = mask,
        .last = cache.front(),
    }).first->second);


//    auto insert_ret = cases.emplace(code, fast_cal_t {
//        .code = code,
//        .mask = mask,
//        .last = cache.front(),
//    });
//
//    if (insert_ret.second) { // insert success
//        cache.emplace(&insert_ret.first->second);
//    } else {
//        insert_ret.first->second.mask |= mask;
//    }


//    cases[code] = fast_cal_t { // insert into cases map
//        .code = code,
//        .mask = mask,
//        .last = cache.front(), // parent case
//    };
//    cache.emplace(&cases[code]); // add in working queue
}

void FastCal::fast_cal(uint64_t code) {

    auto core = Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );

    cases.empty();
    cache.empty();

//    cases[code] = fast_cal_t {
//        .code = code,
//        .mask = 0,
//        .last = nullptr,
//    };
//    cache.emplace(&cases[code]);

//    cases.reserve(30000);
    cases.reserve(65536);
//    cases.reserve(65536 * 2);
//    cases.reserve(65536 * 4);

    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr,
    }).first->second);

    while (!cache.empty()) {

        /// break check point

        if (((cache.front()->code >> (3 * 0xD)) & 0b111) == B_2x2) {
            std::cout << "Resolved" << std::endl;
            break;
        }

        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

    auto solution = cache.front();

    while (solution != nullptr) {
//        printf("%016lX\n", solution->code);
//        std::cout << RawCode(solution->code).dump_case() << std::endl;
        solution = solution->last;
    }

}
