#include "core.h"
#include "common.h"
#include "fast_cal.h"

#include "raw_code.h"

#include <iostream>
#include <algorithm>

Core FastCal::init() { // initialization process
    /// clear working data
    cases.clear();
    std::queue<fast_cal_t*>{}.swap(cache);

    // TODO: test the speed without hashmap reserve
    cases.reserve(65536);

    /// import klotski core
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
    cache.emplace(&cases.emplace(code, fast_cal_t { // record new case
        .code = code,
        .mask = mask,
        .last = cache.front(), // link parent case
    }).first->second);
}

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

uint64_t FastCal::solve(uint64_t code) {
    return FastCal::target(code, [](uint64_t code) {
        return ((code >> (3 * 0xD)) & 0b111) == B_2x2; // check 2x2 block address
    });
}

std::vector<uint64_t> FastCal::solve_multi(uint64_t code) {
    return FastCal::target_multi(code, [](uint64_t code) {
        return ((code >> (3 * 0xD)) & 0b111) == B_2x2; // check 2x2 block address
    });
}

uint64_t FastCal::target(uint64_t code, const check_t &match) {
    auto core = init();

    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr, // without parent node
    }).first->second);

    while (!cache.empty()) {
        if (match(cache.front()->code)) {
            return cache.front()->code; // match target
        }
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }
    return FastCal::NOT_FOUND; // target not found
}

std::vector<uint64_t> FastCal::target_multi(uint64_t code, const FastCal::check_t &match) {

    auto core = init();

    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr, // without parent node
    }).first->second);

    auto layer_end = cache.back();

    int layer_num = 0;

//    bool matched = false;

    std::vector<uint64_t> matched;

    while (!cache.empty()) {

        if (match(cache.front()->code)) {
//            matched = true;
            matched.emplace_back(cache.front()->code);
//            std::cout << "found: " << std::endl;
//            std::cout << RawCode(cache.front()->code);

        }

        core.next_cases(cache.front()->code, cache.front()->mask);

        if (cache.front() == layer_end) {

            if (!matched.empty()) {
                std::cout << "layer " << layer_num << " end -> exit search" << std::endl;
                return matched;
            }

            std::cout << "reach layer " << layer_num << " ending" << std::endl;
            ++layer_num;

            layer_end = cache.back();

        }

        cache.pop();

    }

    return std::vector<uint64_t>{}; // no target found

}

std::vector<uint64_t> FastCal::furthest(uint64_t code) {

    auto core = init();

    cache.emplace(&cases.emplace(code, fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr, // without parent node
    }).first->second);

    auto layer_end = cache.back();

    int layer_num = 0;

    std::vector<uint64_t> layer_cases;

    while (!cache.empty()) {

        core.next_cases(cache.front()->code, cache.front()->mask);

        layer_cases.emplace_back(cache.front()->code);

        if (cache.front() == layer_end) {

            std::cout << "layer size -> " << layer_cases.size() << std::endl;
            std::cout << "reach layer " << layer_num << " ending -> " << cache.size() << std::endl;

            if (cache.size() == 1) { // almost exit -> last layer
                break;
//                return layer_cases;

            }

            layer_cases.clear();

            ++layer_num;

            layer_end = cache.back();

        }

        cache.pop();

    }


    return layer_cases;

}

uint32_t FastCal::step_num(uint64_t code) {

    uint32_t num = 0;

    // TODO: confirm code exist
    auto node = &cases[code];

    while ((node = node->last) != nullptr) {
        ++num;
    }

    return num;
}




