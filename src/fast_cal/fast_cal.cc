#include <algorithm>
#include "core.h"
#include "common.h"
#include "fast_cal.h"
#include "raw_code.h"

/// klotski resolved -> 2x2 block at address 13 (aka 0xD)
RawCode FastCal::solve(RawCode code) {
    return FastCal::target(code, [](uint64_t code) {
        return ((code >> (3 * 0xD)) & 0b111) == B_2x2; // check 2x2 block address
    });
}

std::vector<RawCode> FastCal::solve_multi(RawCode code) {
    return FastCal::target_multi(code, [](uint64_t code) {
        return ((code >> (3 * 0xD)) & 0b111) == B_2x2; // check 2x2 block address
    });
}

/// backtrack of FastCal tree
int FastCal::step_num(RawCode code) {
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

std::vector<RawCode> FastCal::backtrack(RawCode code) {
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
