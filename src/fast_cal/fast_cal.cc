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

uint64_t FastCal::solve() {

    auto resolved = [](uint64_t code) {
        return ((code >> (3 * 0xD)) & 0b111) == B_2x2;
    };
    return target(resolved);

}

// TODO: single backtrack function
std::vector<uint64_t> FastCal::backtrack(uint64_t code) {
    std::vector<uint64_t> path;

    // TODO: confirm code exist
    auto node = &cases[code];

    while (node != nullptr) {
        path.emplace_back(node->code);
        node = node->last;
    }

    std::reverse(path.begin(), path.end());
    return path;

}

uint64_t FastCal::target(const std::function<bool(uint64_t)> &match) {

    auto core = import_core();

    // clear data
    cases.clear();

    std::queue<fast_cal_t*>{}.swap(cache);

//    auto empty = std::queue<fast_cal_t*>{};
//    std::swap(empty, cache);

    cases.reserve(65536);

    cache.emplace(&cases.emplace(root, fast_cal_t {
        .code = root,
        .mask = 0,
        .last = nullptr,
    }).first->second);

    while (!cache.empty()) {

        if (match(cache.front()->code)) {
            break;
        }

        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

    // TODO: cache may empty -> never found
    if (!cache.empty()) {

        return cache.front()->code;

    }
    return FastCal::NOT_FOUND;

}
