#include "core.h"
#include "analyse.h"

#include <iostream>
#include "raw_code.h"

Core Analyse::new_core() {
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

void Analyse::backtrack(uint64_t code) {

    // backtrack start at code
    std::cout << "start backtrack" << std::endl;

    std::cout << RawCode(code).dump_case() << std::endl;
    std::cout << "src size: " << cases[code].src.size() << std::endl;

    auto last_1 = cases[code].src.front();
    std::cout << RawCode(last_1->code).dump_case() << std::endl;
    std::cout << "src size: " << last_1->src.size() << std::endl;

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
//        .highlight = false,
//        .src = std::move(std::list<analyse_t*>{}),
    }).first->second);

    while (!cache.empty()) {

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

}

void Analyse::new_case(uint64_t code, uint64_t mask) {


//    auto temp = cases.emplace(code, analyse_t {
//        .code = code,
//        .mask = mask,
//        .step = cache.front()->step + 1,
//        .src = std::move(std::list<analyse_t*>{cache.front()})
//    });
//
//    if (temp.second) {
//        cache.emplace(&temp.first->second);
//    } else {
//        temp.first->second.mask |= mask;
//
//        if (temp.first->second.step != cache.front()->step) {
//            temp.first->second.src.push_back(cache.front());
//        }
//
//    }

    auto current = cases.find(code);
    if (current != cases.end()) { // find existed case
        current->second.mask |= mask; // update mask info

        if (current->second.step != cache.front()->step) {
            current->second.src.push_back(cache.front());
        }

        return;
    }

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = mask,
        .step = cache.front()->step + 1,
//        .highlight = false,
        .src = std::move(std::list<analyse_t*>{cache.front()}),
    }).first->second);

}
