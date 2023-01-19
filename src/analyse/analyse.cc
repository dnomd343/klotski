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
//            .src = std::vector<analyse_t*>{},
//            .src = std::set<analyse_t*>{},
//            .src = std::unordered_set<analyse_t*>{},
    }).first->second);

    while (!cache.empty()) {
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

}

void Analyse::new_case(uint64_t code, uint64_t mask) {
    auto current = cases.find(code);
    if (current != cases.end()) { // new case already exist
        if (current->second.step == cache.front()->step + 1) { // new case at next layer
            current->second.mask |= mask; // update mask info
            current->second.src.emplace_back(cache.front()); // link more parent case
//            current->second.src.emplace(cache.front()); // link more parent case
        }
    } else { // new case not exist
        cache.emplace(&cases.emplace(code, analyse_t { // record new case
            .code = code,
            .mask = mask,
            .step = cache.front()->step + 1,
                .src = std::list<analyse_t*>{cache.front()}, // link parent case
//                .src = std::vector<analyse_t*>{cache.front()}, // link parent case
//                .src = std::set<analyse_t*>{cache.front()}, // link parent case
//                .src = std::unordered_set<analyse_t*>{cache.front()}, // link parent case
        }).first->second);
    }
}
