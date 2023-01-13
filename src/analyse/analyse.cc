#include "core.h"
#include "analyse.h"

#include <iostream>

Core Analyse::new_core() {
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

void Analyse::start_analyse(uint64_t code) {

    auto core = new_core();

    cases.empty();
    cache.empty();

    cases.reserve(65536);

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = 0,
        .step = 0,
        .src = std::move(std::set<analyse_t*>{}),
    }).first->second);

//    std::cout << "src size: " << cases[code].src.size() << std::endl;

    while (!cache.empty()) {

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

}

void Analyse::new_case(uint64_t code, uint64_t mask) {
    auto current = cases.find(code);
    if (current != cases.end()) { // find existed case
        current->second.mask |= mask; // update mask info

//        current->second.src.emplace()

        if (current->second.step != cache.front()->step) {
            current->second.src.emplace(cache.front());
        }

        return;
    }

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = mask,
        .step = cache.front()->step + 1,
        .src = std::move(std::set<analyse_t*>{cache.front()}),
    }).first->second);
}
