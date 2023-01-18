#include "core.h"
#include "fast_cal.h"

Core FastCal::init(uint64_t code) { // initialize process
    /// clear working data
    cases.clear();
    std::queue<fast_cal_t*>{}.swap(cache);

    // TODO: test the speed without hashmap reserve
    cases.reserve(65536);

    /// insert root node
    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr, // without parent node
    }).first->second);

    /// import klotski core
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

/// callback function for new case
void FastCal::new_case(uint64_t code, uint64_t mask) {
    auto current = cases.find(code);
    if (current != cases.end()) { // find existed case
        current->second.mask |= mask; // update mask info
        return;
    }
    cache.emplace(&cases.emplace(code, fast_cal_t { // record new case
        .code = code,
        .mask = mask,
        .last = cache.front(), // link parent case
    }).first->second);
}

/// found first matched target
uint64_t FastCal::target(uint64_t code, const check_t &match) {
    auto core = init(code);
    while (!cache.empty()) { // bfs search
        if (match(cache.front()->code)) {
            return cache.front()->code; // match target
        }
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }
    return FastCal::NOT_FOUND; // target not found
}

/// found multi-targets matched in first same layer
std::vector<uint64_t> FastCal::target_multi(uint64_t code, const check_t &match) {
    auto core = init(code);
    auto layer_end = cache.back();
    std::vector<uint64_t> matched; // matched list
    while (!cache.empty()) { // bfs search
        if (match(cache.front()->code)) { // match target
            matched.emplace_back(cache.front()->code);
        }
        core.next_cases(cache.front()->code, cache.front()->mask);
        if (cache.front() == layer_end) { // reach layer ending
            if (!matched.empty()) {
                return matched; // stop at first matched layer
            }
            layer_end = cache.back(); // reset layer ending
        }
        cache.pop();
    }
    return std::vector<uint64_t>{}; // no target found
}

/// found all of the furthest cases
std::vector<uint64_t> FastCal::furthest(uint64_t code) {
    auto core = init(code);
    auto layer_end = cache.back();
    std::vector<uint64_t> layer_cases;
    while (!cache.empty()) { // bfs search
        core.next_cases(cache.front()->code, cache.front()->mask);
        layer_cases.emplace_back(cache.front()->code); // record layer cases
        if (cache.front() == layer_end) { // reach layer ending
            if (cache.size() == 1) {
                break; // stop loop at last layer
            }
            layer_cases.clear();
            layer_end = cache.back(); // reset layer ending
        }
        cache.pop();
    }
    return layer_cases; // release latest layer cases
}
