#include <algorithm>
#include "core.h"
#include "common.h"
#include "fast_cal.h"
#include "raw_code.h"

auto resolved = [](uint64_t code) {
    return ((code >> (3 * 0xD)) & 0b111) == B_2x2; // check 2x2 block address
};

/// klotski resolved -> 2x2 block at address 13 (aka 0xD)
RawCode FastCal::solve(RawCode code) {
    return FastCal::target(code, resolved);
}

std::vector<RawCode> FastCal::solve_multi(RawCode code) {
    return FastCal::target_multi(code, resolved);
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

std::vector<RawCode> FastCal::resolve(RawCode start) {
    return FastCal::search(start, resolved);
}

std::vector<std::vector<RawCode>> FastCal::resolve_multi(RawCode start) {
    return FastCal::search_multi(start, resolved);
}

std::vector<std::vector<RawCode>> FastCal::to_furthest(RawCode start) {
    auto fc = FastCal();
    std::vector<std::vector<RawCode>> result;
    for (const auto &furthest : fc.furthest(start)) {
        result.emplace_back(fc.backtrack(furthest)); // backtrack every furthest cases
    }
    return result;
}

std::vector<RawCode> FastCal::search(RawCode start, const match_t &match) {
    auto fc = FastCal();
    auto result = fc.target(start, match);
    if (result == FC_NOT_FOUND) {
        return std::vector<RawCode>{}; // target not matched
    }
    return fc.backtrack(result); // backtrack target path
}

std::vector<std::vector<RawCode>> FastCal::search_multi(RawCode start, const match_t &match) {
    auto fc = FastCal();
    std::vector<std::vector<RawCode>> result;
    for (const auto &target : fc.target_multi(start, match)) {
        result.emplace_back(fc.backtrack(target)); // backtrack every target
    }
    return result;
}
