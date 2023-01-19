#pragma once

#include <queue>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "raw_code.h"

/// FastCal not found -> return invalid raw code
const RawCode FC_NOT_FOUND = RawCode::unsafe_create(0);

class FastCal {
public:
    const static auto NOT_FOUND = (uint64_t)0;
    typedef std::function<bool(uint64_t)> check_t;

//    const static RawCode *test = (RawCode*)nullptr;

    /// xxx_multi only search until same layer

    // TODO: shall we use RawCode instead of uint64_t?

    RawCode solve(RawCode code);
    RawCode target(RawCode code, const check_t &match);

    std::vector<RawCode> furthest(RawCode code);

    std::vector<RawCode> solve_multi(RawCode code);
    std::vector<RawCode> target_multi(RawCode code, const check_t &match);

    // TODO: search / search_multi / resolve / resolve_multi
    // TODO: static furthest function

    std::vector<uint64_t> backtrack(uint64_t code);

    uint32_t step_num(uint64_t code);

    // TODO: static search functions


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
