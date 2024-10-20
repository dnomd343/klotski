#include "fast_cal/fast_cal.h"

MaskMover FastCal::init(uint64_t code) { // initialize process
    /// reset working data
    cases.clear();
    cases.reserve(FC_MAP_RESERVE); // hashmap pre-reserve
    std::queue<fast_cal_t*>{}.swap(cache);

    /// insert root node
    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr, // without parent node
    }).first->second);

    /// import klotski mover
    return MaskMover(
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

/// build total search tree
void FastCal::build() {
    auto core = init(root);
    while (!cache.empty()) {
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }
}

/// found first matched target
RawCode FastCal::target(const match_t &match) {
    auto core = init(root);
    while (!cache.empty()) {
        if (match(cache.front()->code)) {
            return RawCode::unsafe_create(cache.front()->code); // match target
        }
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }
    return FC_NOT_FOUND; // target not found
}

/// found all of the furthest cases
std::vector<RawCode> FastCal::furthest() {
    auto core = init(root);
    auto layer_end = cache.back();
    std::vector<RawCode> layer_cases;
    /// start BFS search
    while (!cache.empty()) {
        core.next_cases(cache.front()->code, cache.front()->mask);
        layer_cases.emplace_back(
            RawCode::unsafe_create(cache.front()->code) // record layer cases
        );
        if (cache.front() == layer_end) { // reach layer ending
            if (cache.size() == 1) {
                break; // stop loop at last layer
            }
            layer_cases.clear();
            layer_end = cache.back(); // reset layer ending
        }
        cache.pop();
    }
    return layer_cases; // release the latest layer cases
}

/// found multi-targets matched in first same layer
std::vector<RawCode> FastCal::target_multi(const match_t &match) {
    auto core = init(root);
    auto layer_end = cache.back();
    std::vector<RawCode> matched; // matched list
    /// start BFS search
    while (!cache.empty()) {
        if (match(cache.front()->code)) { // match target
            matched.emplace_back(
                RawCode::unsafe_create(cache.front()->code) // record matched cases
            );
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
    return std::vector<RawCode>{}; // no target found
}
