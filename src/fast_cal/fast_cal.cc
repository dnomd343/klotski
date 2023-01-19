#include "core.h"
#include "common.h"
#include "fast_cal.h"

#include "raw_code.h"

#include <iostream>
#include <algorithm>

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



uint32_t FastCal::step_num(uint64_t code) {

    uint32_t num = 0;

    // TODO: confirm code exist
    auto node = &cases[code];

    while ((node = node->last) != nullptr) {
        ++num;
    }

    return num;
}
