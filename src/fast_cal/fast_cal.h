#pragma once

#include <queue>
#include <cstdint>
#include <unordered_map>

#include <vector>

class FastCal {
public:

    typedef std::function<bool(uint64_t)> check_t;

    const static auto NOT_FOUND = (uint64_t)0;

//    explicit FastCal(uint64_t code) : root(code) {}

    // search resolve
    // search all min-step resolve
    // search the furthest cases
    // search target by code
    // search target by lambda

    /// xxx_multi only search until same layer

//    solve_multi

    // TODO: shall we using RawCode instead of uint64_t?

    uint64_t solve(uint64_t code);

    uint64_t target(uint64_t code, const check_t &match);

    std::vector<uint64_t> solve_multi(uint64_t code);

    std::vector<uint64_t> target_multi(uint64_t code, const check_t &match);

    std::vector<uint64_t> furthest(uint64_t code);

    std::vector<uint64_t> backtrack(uint64_t code);

    uint32_t step_num(uint64_t code);

    // TODO: static search functions


private:
    struct fast_cal_t {
        uint64_t code;
        uint64_t mask;
        fast_cal_t *last;
    };


//    uint64_t root;

    std::queue<fast_cal_t*> cache;
    std::unordered_map<uint64_t, fast_cal_t> cases;

    Core init();

    void new_case(uint64_t code, uint64_t mask);

};
