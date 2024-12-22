#include "analyse.h"
#include "utils/common.h"

using klotski::Analyse;
using klotski::codec::RawCode;
using klotski::mover::MaskMover;

Analyse::Analyse(const RawCode &code) {
    this->root = (uint64_t)code;
}

void Analyse::set_root(const RawCode &code) {
    this->root = (uint64_t)code;
}

std::vector<RawCode> Analyse::layer_export(uint32_t layer_num) {
    std::vector<RawCode> layer_cases;
    for (const auto &tmp : cases) { // traverse every cases
        if (tmp.second.step == layer_num) {
            layer_cases.emplace_back(RawCode::unsafe_create(tmp.first));
        }
    }
    return layer_cases;
}

std::vector<std::vector<RawCode>> Analyse::layer_export() {
    std::vector<std::vector<RawCode>> layer_cases;
    for (const auto &tmp : cases) { // traverse every cases
        if (tmp.second.step >= layer_cases.size()) {
            layer_cases.resize(tmp.second.step + 1);
        }
        layer_cases[tmp.second.step].emplace_back(
            RawCode::unsafe_create(tmp.first) // insert at target layer
        );
    }
    return layer_cases;
}

std::vector<RawCode> Analyse::build_resolve() {
    return build_until([](uint64_t code) {
        return ((code >> (3 * 0xD)) & 0b111) == BLOCK_2x2;
    });
}

/// memory initialize and return klotski core
MaskMover Analyse::init(uint64_t code) {
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
    return MaskMover(
        [this](RawCode code, uint64_t mask) { // lambda as function pointer
            new_case(code.unwrap(), mask);
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
        core.next_cases(RawCode::unsafe_create(cache.front()->code), cache.front()->mask);
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
        core.next_cases(RawCode::unsafe_create(cache.front()->code), cache.front()->mask);
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
