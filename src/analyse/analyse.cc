#include <queue>
#include "core.h"
#include "analyse.h"

#include <list>
#include <iostream>
#include "raw_code.h"
#include "common.h"

Core Analyse::init() {
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}



void Analyse::build(uint64_t code) {

    auto core = init();

    cases.clear();
    cases.reserve(65536);

    std::queue<analyse_t*>{}.swap(cache);

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = 0,
        .step = 0,
        .src = std::list<analyse_t*>{},
    }).first->second);

    while (!cache.empty()) {
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

}

std::vector<uint64_t> Analyse::build_until(uint64_t code, const Analyse::match_t &match) {

    auto core = init();

    cases.clear();
    cases.reserve(65536);

    std::queue<analyse_t*>{}.swap(cache);

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = 0,
        .step = 0,
        .src = std::list<analyse_t*>{},
    }).first->second);


    auto layer_end = cache.back();
    std::vector<uint64_t> matched; // matched list


    while (!cache.empty()) {
        if (match(cache.front()->code)) { // match target
            matched.emplace_back(cache.front()->code);
        }

        core.next_cases(cache.front()->code, cache.front()->mask);

        if (cache.front() == layer_end) { // reach layer ending
            if (!matched.empty()) {

                std::cout << "size: " << cases.size() << std::endl;

                return matched; // stop at first matched layer
            }
            layer_end = cache.back(); // reset layer ending
        }

        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

    return std::vector<uint64_t>{}; // no target found

}


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

