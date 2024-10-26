#include <algorithm>

#include <iostream>

#include "utils/common.h"
#include "fast_cal/fast_cal.h"
#include "raw_code/raw_code.h"

#include "group/group.h"

FastCal::FastCal(const RawCode &code) {
    this->root = (uint64_t)code;
}

void FastCal::set_root(const RawCode &code) {
    this->root = (uint64_t)code;
}

/// klotski resolved -> 2x2 block at address 13 (aka 0xD)
auto resolved = [](uint64_t code) {
    return ((code >> (3 * 0xD)) & 0b111) == BLOCK_2x2; // check 2x2 block address
};

RawCode FastCal::solve() {
    return FastCal::target(resolved);
}

RawCode FastCal::demo() {

    // max_group_size = 25955
//    auto reserve = klotski::cases::GroupUnion::from_raw_code(RawCode::unsafe_create(root)).max_group_size();

    cases.reserve(25955); // FAST !!! (about 5ms)
//    cases.reserve(32768); // SLOW !!! (about 50ms)

    // TODO: using prime number!

    std::queue<fast_cal_t*>{}.swap(cache);

    cache.emplace(&cases.emplace(root, fast_cal_t {
        .code = root,
        .mask = 0,
        .last = nullptr, // without parent node
    }).first->second);

    auto core = MaskMover(
        [this](uint64_t code, uint64_t mask) { // lambda as function pointer

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
    );

    while (!cache.empty()) {
        if (((cache.front()->code >> (3 * 0xD)) & 0b111) == BLOCK_2x2) {
            return RawCode::unsafe_create(cache.front()->code); // match target
        }
        core.next_cases(cache.front()->code, cache.front()->mask);
        cache.pop();
    }
    return FC_NOT_FOUND; // target not found
}

std::vector<RawCode> FastCal::solve_multi() {
    return FastCal::target_multi(resolved);
}

std::vector<RawCode> FastCal::resolve(const RawCode &start) {
    return FastCal::search(start, resolved);
}

std::vector<std::vector<RawCode>> FastCal::resolve_multi(const RawCode &start) {
    return FastCal::search_multi(start, resolved);
}

/// backtrack of FastCal tree
int FastCal::step_num(const RawCode &code) {
    auto tmp = cases.find((uint64_t)code);
    if (tmp == cases.end()) {
        return -1; // code not exist
    }
    int num = 0; // step number
    auto node = &tmp->second; // backtrack entry
    while ((node = node->last) != nullptr) {
        ++num;
    }
    return num;
}

std::vector<RawCode> FastCal::backtrack(const RawCode &code) {
    auto tmp = cases.find((uint64_t)code);
    if (tmp == cases.end()) {
        return std::vector<RawCode>{}; // code not exist
    }
    auto node = &tmp->second; // backtrack entry
    std::vector<RawCode> path; // backtrack path
    while (node != nullptr) {
        path.emplace_back(RawCode::unsafe_create(node->code)); // record path info
        node = node->last;
    }
    std::reverse(path.begin(), path.end()); // reverse path cases
    return path;
}

/// static BFS search functions
std::vector<std::vector<RawCode>> FastCal::to_furthest(const RawCode &start) {
    auto fc = FastCal(start);
    std::vector<std::vector<RawCode>> result;
    for (const auto &furthest : fc.furthest()) {
        result.emplace_back(fc.backtrack(furthest)); // backtrack every furthest cases
    }
    return result;
}

std::vector<RawCode> FastCal::search(const RawCode &start, const match_t &match) {
    auto fc = FastCal(start);
    auto result = fc.target(match);
    if (result == FC_NOT_FOUND) {
        return std::vector<RawCode>{}; // target not matched
    }
    return fc.backtrack(result); // backtrack target path
}

std::vector<std::vector<RawCode>> FastCal::search_multi(const RawCode &start, const match_t &match) {
    auto fc = FastCal(start);
    std::vector<std::vector<RawCode>> result;
    for (const auto &target : fc.target_multi(match)) {
        result.emplace_back(fc.backtrack(target)); // backtrack every target
    }
    return result;
}
