/// Klotski Engine by Dnomd343 @2024

// TODO: only copy from old implementation, the interfaces will change in future.

#pragma once

#include <queue>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "mover/mover.h"
#include "raw_code/raw_code.h"

using klotski::codec::RawCode;
using klotski::mover::MaskMover;

// TODO: using prime number
const uint32_t FC_MAP_RESERVE = 65536 * 8;

/// FastCal not found -> return invalid raw code
const RawCode FC_NOT_FOUND = RawCode::unsafe_create(0);

class FastCal {
public:
    typedef std::function<bool(uint64_t)> match_t;

    /// setting root code
    void set_root(const RawCode &code);
    explicit FastCal(const RawCode &code);

    /// backtrack functions
    int step_num(const RawCode &code);
    std::vector<RawCode> backtrack(const RawCode &code);

    /// BFS search functions
    void build();
    RawCode solve();
    std::vector<RawCode> furthest();
    std::vector<RawCode> solve_multi();
    RawCode target(const match_t &match);
    std::vector<RawCode> target_multi(const match_t &match);

    /// static BFS search functions
    static std::vector<RawCode> resolve(const RawCode &start);
    static std::vector<std::vector<RawCode>> to_furthest(const RawCode &start);
    static std::vector<std::vector<RawCode>> resolve_multi(const RawCode &start);
    static std::vector<RawCode> search(const RawCode &start, const match_t &match);
    static std::vector<std::vector<RawCode>> search_multi(const RawCode &start, const match_t &match);

private:
    struct fast_cal_t {
        uint64_t code;
        uint64_t mask;
        fast_cal_t *last;
    };

    uint64_t root;
    std::queue<fast_cal_t*> cache;
    std::unordered_map<uint64_t, fast_cal_t> cases;

    inline MaskMover init(uint64_t code);
    void new_case(uint64_t code, uint64_t mask);
};
