#include "analyse.h"

Analyse::Analyse(const RawCode &code) {
    this->root = (uint64_t)code;
}

void Analyse::set_root(const RawCode &code) {
    this->root = (uint64_t)code;
}

/// memory initialize and return klotski core
Core Analyse::init(uint64_t code) {
    /// reset working data
    cases.clear();
    cases.reserve(ANY_MAP_RESERVE); // hashmap pre-reserve
    std::queue<analyse_t*>{}.swap(cache);

    /// insert root node
    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = 0,
        .step = 0,
        .src = std::list<analyse_t*>{}, // without parent node
    }).first->second);

    /// import klotski core
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

/// callback function for new case
void Analyse::new_case(uint64_t code, uint64_t mask) {
    auto current = cases.find(code);
    if (current != cases.end()) { // new case already exist
        if (current->second.step == cache.front()->step + 1) { // new case at next layer
            current->second.mask |= mask; // update mask info
            current->second.src.emplace_back(cache.front()); // link more parent case
        }
    } else { // new case not exist
        cache.emplace(&cases.emplace(code, analyse_t { // record new case
            .code = code,
            .mask = mask,
            .step = cache.front()->step + 1,
            .src = std::list<analyse_t*>{cache.front()}, // link parent case
        }).first->second);
    }
}

/// analyse and build klotski tree
void Analyse::build() {
    auto core = init(root);
    while (!cache.empty()) {
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }
}

std::vector<RawCode> Analyse::build_until(const match_t &match) {
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
