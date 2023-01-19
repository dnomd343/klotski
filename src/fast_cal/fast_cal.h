#pragma once

#include <queue>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "raw_code.h"

const uint32_t FC_MAP_RESERVE = 65536;

/// FastCal not found -> return invalid raw code
const RawCode FC_NOT_FOUND = RawCode::unsafe_create(0);

class FastCal {
public:
    typedef std::function<bool(uint64_t)> match_t;

    /// backtrack functions
    int step_num(RawCode code);
    std::vector<RawCode> backtrack(const RawCode &code);

    /// BFS search functions
    // TODO: build function with void return -> build total tree
    RawCode solve(RawCode code);
    std::vector<RawCode> furthest(RawCode code);
    std::vector<RawCode> solve_multi(RawCode code);
    RawCode target(RawCode code, const match_t &match);
    std::vector<RawCode> target_multi(RawCode code, const match_t &match);

    /// static BFS search functions
    // TODO: (RawCode code) -> (const RawCode &code)
//    static std::vector<RawCode> get_furthest(RawCode start);

    static std::vector<RawCode> resolve(RawCode start);
    static std::vector<RawCode> search(RawCode start, const match_t &match);

    static std::vector<std::vector<RawCode>> resolve_multi(RawCode start);
    static std::vector<std::vector<RawCode>> search_multi(RawCode start, const match_t &match);

private:
    struct fast_cal_t {
        uint64_t code;
        uint64_t mask;
        fast_cal_t *last;
    };

    std::queue<fast_cal_t*> cache;
    std::unordered_map<uint64_t, fast_cal_t> cases;

    Core init(uint64_t code);
    void new_case(uint64_t code, uint64_t mask);
};
