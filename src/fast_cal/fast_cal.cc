#include "core.h"
#include "common.h"
#include "fast_cal.h"

#include "raw_code.h"

#include <iostream>
#include <algorithm>

Core FastCal::import_core() {
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

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

}

std::vector<uint64_t> FastCal::search(uint64_t code) {

    auto core = import_core();

    // clear data
    cases.clear();

    auto empty = std::queue<fast_cal_t*>{};
    std::swap(empty, cache);


    cases.reserve(65536);


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

    std::vector<uint64_t> solution_path;

    while (solution != nullptr) {
        solution_path.emplace_back(solution->code);
        solution = solution->last;
    }

    std::reverse(solution_path.begin(), solution_path.end());
    return solution_path;

}
